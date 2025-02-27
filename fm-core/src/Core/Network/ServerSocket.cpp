#include "fmpch.h"

#include <enet.h>

#include "ServerSocket.h"

namespace fm {

	struct ServerSocketData {

		ENetHost* Host = { 0 };
		bool Bound = false;
		std::unordered_map<Uint32, Socket*> Sockets;
		std::condition_variable_any AcceptNotifier;
		ENetPeer* NewPeerPtr = { 0 };
		bool NewConnection = false;
		Socket::Status Status = Socket::Status::Done;
	};

	ServerSocket::ServerSocket(Uint16 port)
		: m_Mutex(), m_Thread(), m_Data(CreateScope<ServerSocketData>())

	{

		Socket::Init();
		Bind(port);
	}

	ServerSocket::ServerSocket()
		: m_Mutex(), m_Thread(), m_Data(CreateScope<ServerSocketData>())

	{

		Socket::Init();
	}

	ServerSocket::~ServerSocket() {

		Unbind();
		Socket::Shutdown();
	}

	Socket::Status ServerSocket::Bind(Uint16 port) {

		if (IsBound())
			Unbind();

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		ENetAddress address = { 0 };
		address.host = ENET_HOST_ANY;
		address.port = port;
		m_Data->Host = enet_host_create(&address, 32, 2, 0, 0);
		m_Data->Bound = m_Data->Host != NULL;
		m_Thread = CreateScope<std::thread>(&ServerSocket::PollEvents, this);

		return m_Data->Bound ? Socket::Status::Done : Socket::Status::Error;
	}

	void ServerSocket::Unbind() {

		if (!IsBound())
			return;

		{

			std::lock_guard<std::recursive_mutex> lock(m_Mutex);

			m_Data->Bound = false;
		}

		if (m_Thread->joinable())
			m_Thread->join();

		if (m_Data->Host != NULL) {

			enet_host_destroy(m_Data->Host);
			m_Data->Host = NULL;
		}

		m_Data = CreateScope<ServerSocketData>();
	}

	bool ServerSocket::IsBound() const {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		return m_Data->Bound;
	}

	Socket::Status ServerSocket::Accept(Socket* socket) {

		if (!IsBound())
			return Socket::Status::NotReady;

		std::unique_lock<std::recursive_mutex> lock(m_Mutex);

		while (!m_Data->NewConnection) {

			m_Data->AcceptNotifier.wait(lock);
			if (!IsBound())
				return m_Data->Status;
		}

		char buf[2048];
		enet_address_get_host_ip_new(&m_Data->NewPeerPtr->address, buf, 2048);
		const IpAddress address(buf);
		const Uint16& port = m_Data->NewPeerPtr->address.port;

		socket->Disconnect();
		socket->Create(address, port, m_Data->NewPeerPtr);
		m_Data->Sockets[m_Data->NewPeerPtr->incomingPeerID] = socket;
		m_Data->NewPeerPtr = NULL;
		m_Data->NewConnection = false;

		return Socket::Status::Done;
	}

	void ServerSocket::PollEvents() {

		while (IsBound()) {

			ENetEvent event = {};
			int status;
			while (status = enet_host_service(m_Data->Host, &event, 2000) > 0) {

				switch (event.type) {

					case ENET_EVENT_TYPE_CONNECT: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						m_Data->Sockets.emplace((Uint32)event.peer->incomingPeerID, nullptr);
						m_Data->NewPeerPtr = event.peer;
						m_Data->NewConnection = true;
						m_Data->Status = Socket::Status::Done;
						m_Data->AcceptNotifier.notify_all();
						break;
					}

					case ENET_EVENT_TYPE_RECEIVE: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						Socket& socket = *m_Data->Sockets.at(event.peer->incomingPeerID);
						socket.SetReceivedData(event.packet, event.packet->dataLength);
						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						Socket& socket = *m_Data->Sockets.at(event.peer->incomingPeerID);
						socket.Destroy(Socket::Status::Disconnected);
						m_Data->Sockets.erase(event.peer->incomingPeerID);
						break;
					}

					case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: {

						std::lock_guard<std::recursive_mutex> lock(m_Mutex);

						Socket& socket = *m_Data->Sockets.at(event.peer->incomingPeerID);
						socket.Destroy(Socket::Status::Timeout);
						m_Data->Sockets.erase(event.peer->incomingPeerID);
						break;
					}

					case ENET_EVENT_TYPE_NONE: {}
						break;
				}
			}

			if (status < 0) {

				std::lock_guard<std::recursive_mutex> lock(m_Mutex);

				m_Data->Bound = false;
				m_Data->Status = Socket::Status::Error;
			}
		}

		enet_host_destroy(m_Data->Host);
		m_Data->Host = NULL;
	}
}
