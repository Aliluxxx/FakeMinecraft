#include "fmpch.h"

#include <enet.h>

#include "Socket.h"

namespace fm {

	std::atomic<Uint32> Socket::s_ENetInstances = 0;

	struct SocketData {

		ENetHost* Host = { 0 };
		ENetHost* ServerHost = { 0 };
		ENetPeer* Peer = { 0 };
		bool Connected = false;
		IpAddress RemoteAddress = IpAddress::Invalid;
		Uint16 RemotePort = 0;
		std::condition_variable_any ReceiveNotifier;
		Packet ReceivedPacket;
		Socket::Status Status = Socket::Status::NotReady;
		std::condition_variable_any PingNotifier;
		bool WasPinged = false;
		Uint32 PingEndTime = 0;
	};

	Socket::Socket(IpAddress address, Uint16 port, Uint32 channels_count)
		: m_Mutex(), m_Thread(), m_Data(CreateScope<SocketData>())

	{

		Socket::Init();
		Connect(address, port);
	}

	Socket::Socket()
		: m_Mutex(), m_Thread(), m_Data(CreateScope<SocketData>())

	{

		Socket::Init();
	}

	Socket::~Socket() {

		Disconnect();
		Socket::Shutdown();
	}

	Socket::Status Socket::Connect(IpAddress address, Uint16 port, Uint32 channels_count) {

		if (IsConnected())
			Disconnect();

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		m_Data->Host = enet_host_create(NULL /* create a client host */,
			1 /* only allow 1 outgoing connection */,
			channels_count < MINIMUM_CHANNEL_COUNT ? MINIMUM_CHANNEL_COUNT : channels_count /* allow up 2 channels to be used, 0 and 1 */,
			0 /* assume any amount of incoming bandwidth */,
			0 /* assume any amount of outgoing bandwidth */);

		if (m_Data->Host == NULL)
			return Status::Error;

		ENetAddress ip = { 0 };
		ENetEvent event = {};

		int a = enet_address_set_host_new(&ip, address.ToString().c_str());
		ip.port = port;

		m_Data->Peer = enet_host_connect(m_Data->Host, &ip, MINIMUM_CHANNEL_COUNT, 0);
		if (enet_host_service(m_Data->Host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {

			char buf[2048];
			enet_address_get_host_ip_new(&event.peer->address, buf, sizeof(buf) / sizeof(char));
			m_Data->RemoteAddress = IpAddress(buf);
			m_Data->RemotePort = event.peer->address.port;
			m_Data->Connected = true;
			if (m_Thread && m_Thread->joinable())
				m_Thread->join();
			m_Thread = CreateScope<std::thread>(&Socket::PollEvents, this);
		}

		else {

			enet_peer_reset(m_Data->Peer);
			enet_host_destroy(m_Data->Host);
			m_Data->Peer = NULL;
			m_Data->Host = NULL;
		}

		return m_Data->Connected ? Status::Done : Status::Timeout;
	}

	bool Socket::IsConnected() const {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		return m_Data->Connected;
	}

	Socket::Status Socket::Send(const Packet& packet, PacketFlags_ flags, Uint32 channel) {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		if (!IsConnected())
			return m_Data->Status;

		ENetPacket* p = enet_packet_create(packet.GetData(), packet.GetDataSize(), flags);

		if (enet_peer_send(m_Data->Peer, channel, p) == 0)
			return Status::Done;

		else {

			enet_packet_destroy(p);

			if (!IsConnected())
				return m_Data->Status;

			return Status::Error;
		}
	}

	Socket::Status Socket::Receive(Packet* packet) {

		if (!IsConnected())
			return m_Data->Status;

		std::unique_lock<std::recursive_mutex> lock(m_Mutex);

		while (m_Data->ReceivedPacket.GetDataSize() == 0) {

			m_Data->ReceiveNotifier.wait(lock);
			if (!IsConnected())
				return m_Data->Status;
		}

		*packet = std::move(m_Data->ReceivedPacket);

		return Status::Done;
	}

	Time Socket::Ping(IpAddress address, Uint16 port) {

		if (!m_Data->Connected || address.ToInteger() != m_Data->RemoteAddress.ToInteger() || port != m_Data->RemotePort) {

			ENetHost* host = enet_host_create(NULL /* create a client host */,
				1 /* only allow 1 outgoing connection */,
				2 /* allow up 2 channels to be used, 0 and 1 */,
				0 /* assume any amount of incoming bandwidth */,
				0 /* assume any amount of outgoing bandwidth */);

			if (host == NULL)
				return Time::Infinity;

			ENetAddress ip = { 0 };
			ENetEvent event = {};

			enet_address_set_host_new(&ip, address.ToString().c_str());
			ip.port = port;

			ENetPeer* peer = enet_host_connect(host, &ip, MINIMUM_CHANNEL_COUNT, PING_REQ);

			if (peer == NULL)
				return Time::Infinity;

			if (enet_host_service(host, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {

				Uint32 start = enet_time_get();
				Uint32 end = 0;
				Packet p;
				p << CLIENT_PING;
				ENetPacket* packet = enet_packet_create(p.GetData(), p.GetDataSize(), ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer, CONTROL_CHANNEL, packet);
				if (enet_host_service(host, &event, 3000) > 0) {

					if (event.type == ENET_EVENT_TYPE_RECEIVE) {

						end = enet_time_get();
						enet_packet_destroy(event.packet);
						enet_peer_disconnect(peer, PING_REQ);
						enet_host_flush(host);
						enet_host_destroy(host);
					}
				}

				else
					return Time::Infinity;

				return fm::Milliseconds(Int32(end - start));
			}

			else {

				enet_peer_reset(peer);
				enet_host_destroy(host);
			}

			return Time::Infinity;
		}

		else if (m_Data->Connected) {

			std::unique_lock<std::recursive_mutex> lock(m_Mutex);

			// Client-Side
			if (m_Data->Host != NULL) {

				Packet p;
				p << CLIENT_PING;
				m_Data->WasPinged = false;
				Send(p, PacketFlags::Reliable, CONTROL_CHANNEL);
				Uint32 start = enet_time_get();
				m_Data->PingNotifier.wait_for(lock, std::chrono::milliseconds(5000));

				if (m_Data->WasPinged)
					return fm::Milliseconds(Int32(m_Data->PingEndTime - start));

				else
					return Time::Infinity;
			}

			// Server-Side
			else if (m_Data->ServerHost != NULL) {

				Packet p;
				p << SERVER_PING;
				m_Data->WasPinged = false;
				Send(p, PacketFlags::Reliable, CONTROL_CHANNEL);
				Uint32 start = enet_time_get();
				m_Data->PingNotifier.wait_for(lock, std::chrono::milliseconds(5000));

				if (m_Data->WasPinged)
					return fm::Milliseconds(Int32(m_Data->PingEndTime - start));

				else
					return Time::Infinity;
			}
		}

		return Time::Infinity;
	}

	void Socket::Flush() {

		if (m_Data->Host != NULL)
			enet_host_flush(m_Data->Host);
		else
			enet_host_flush(m_Data->ServerHost);
	}

	void Socket::Disconnect() {

		if (m_Data->Host != NULL) {

			{

				std::lock_guard<std::recursive_mutex> lock(m_Mutex);

				m_Data->Connected = false;
			}

			if (m_Thread->joinable())
				m_Thread->join();

			std::lock_guard<std::recursive_mutex> lock(m_Mutex);

			enet_peer_disconnect(m_Data->Peer, 0);

			ENetEvent event = {};
			bool disconnected = false;
			while (enet_host_service(m_Data->Host, &event, 100) > 0) {

				switch (event.type) {

					case ENET_EVENT_TYPE_RECEIVE:
						enet_packet_destroy(event.packet);
						break;

					case ENET_EVENT_TYPE_DISCONNECT:
						disconnected = true;
						break;
				}
			}

			if (!disconnected)
				enet_peer_reset(m_Data->Peer);

			enet_host_destroy(m_Data->Host);

			m_Data->Peer = NULL;
			m_Data->Host = NULL;

			m_Data->Connected = false;
			m_Data->Status = Status::Disconnected;
			m_Data->ReceiveNotifier.notify_all();
		}

		else if (IsConnected()) {

			std::lock_guard<std::recursive_mutex> lock(m_Mutex);

			enet_peer_disconnect_later(m_Data->Peer, 0);
		}
	}

	IpAddress Socket::GetRemoteAddress() const {

		std::unique_lock<std::recursive_mutex> lock(m_Mutex);

		return m_Data->RemoteAddress;
	}

	Uint16 Socket::GetRemotePort() const {

		std::unique_lock<std::recursive_mutex> lock(m_Mutex);

		return m_Data->RemotePort;
	}

	void Socket::Init() {

		if (s_ENetInstances == 0) {

			int result = enet_initialize();
			FM_CORE_ASSERT(result == 0);
			s_ENetInstances++;
		}

		else
			s_ENetInstances++;
	}

	void Socket::Shutdown() {

		if (s_ENetInstances > 1)
			s_ENetInstances--;

		else {

			s_ENetInstances--;
			enet_deinitialize();
		}
	}

	void Socket::PollEvents() {

		while (IsConnected()) {

			ENetEvent event = {};
			while (enet_host_service(m_Data->Host, &event, 1) > 0) {

				switch (event.type) {

					case ENET_EVENT_TYPE_RECEIVE: {

						if (event.channelID != CONTROL_CHANNEL)
							SetReceivedData(event.packet, event.packet->dataLength);

						else {

							Packet p; p.Append(event.packet->data, event.packet->dataLength);
							Uint32 controlData = 0;
							p >> controlData;

							if (controlData == CLIENT_PING) {

								enet_packet_destroy(event.packet);
								SignalPing();
							}

							else if (controlData == SERVER_PING) {

								enet_peer_send(event.peer, CONTROL_CHANNEL, event.packet);
							}

							else {

								enet_packet_destroy(event.packet);
							}
						}

						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						m_Data->Connected = false;
						m_Data->Status = Socket::Status::Disconnected;
						m_Data->ReceiveNotifier.notify_all();

						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						m_Data->Connected = false;
						m_Data->Status = Socket::Status::Timeout;
						m_Data->ReceiveNotifier.notify_all();
						break;
					}
				}
			}
		}
	}

	void Socket::Create(IpAddress address, Uint16 port, void* host, void* peer) {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		m_Data->Connected = true;
		m_Data->Host = NULL;
		m_Data->ServerHost = (ENetHost*)host;
		m_Data->Peer = (ENetPeer*)peer;
		m_Data->RemoteAddress = address;
		m_Data->RemotePort = port;
	}

	void Socket::SetReceivedData(void* packet, std::size_t size) {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		m_Data->ReceivedPacket.Append(((ENetPacket*)packet)->data, ((ENetPacket*)packet)->dataLength);
		enet_packet_destroy(((ENetPacket*)packet));

		m_Data->Status = Socket::Status::Done;
		m_Data->ReceiveNotifier.notify_all();
	}

	void Socket::SignalPing() {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);
		m_Data->WasPinged = true;
		m_Data->PingEndTime = enet_time_get();
		m_Data->PingNotifier.notify_all();
	}

	void Socket::Destroy(Socket::Status status) {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		m_Data->Peer = NULL;
		m_Data->Status = status;
		if (m_Data->Connected) {

			m_Data->Connected = false;
			m_Data->ReceiveNotifier.notify_all();
		}
	}
}
