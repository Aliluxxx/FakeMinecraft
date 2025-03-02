#pragma once

#include <mutex>
#include <atomic>

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"
#include "Core/Network/IpAddress.h"

#define MINIMUM_CHANNEL_COUNT 2u
#define CONTROL_CHANNEL 0u
#define PAYLOAD_CHANNEL 1u
#define CLIENT_PING 1u
#define SERVER_PING 2u
#define PING_REQ 1u

namespace fm {

	class ServerSocket;
	struct SocketData;

	class FM_API Socket {

	public:

		enum class Status {

			Done,
			NotReady,
			Error,
			Disconnected,
			Timeout
		};

		Socket(IpAddress address, Uint16 port, Time timeout = Milliseconds(5000), Uint32 channels_count = MINIMUM_CHANNEL_COUNT);
		Socket(const Socket& other) = delete;
		Socket();
		virtual ~Socket();

		Status Connect(IpAddress address, Uint16 port, Time timeout = Milliseconds(5000), Uint32 channels_count = MINIMUM_CHANNEL_COUNT);
		bool IsConnected() const;
		Status Send(const Packet& packet, PacketFlags_ flags = 0, Uint32 channel = PAYLOAD_CHANNEL);
		Status Receive(Packet* packet);
		Time Ping(IpAddress address, Uint16 port);
		void Flush();
		void Disconnect();
		IpAddress GetRemoteAddress() const;
		Uint16 GetRemotePort() const;

	private:

		static void Init();
		static void Shutdown();

		static std::atomic<Uint32> s_ENetInstances;

		void PollEvents();
		void Create(IpAddress address, Uint16 port, void* host, void* peer);
		void SetReceivedData(void* packet, std::size_t size);
		void SignalPing();
		void Destroy(Socket::Status status);

		mutable std::recursive_mutex m_Mutex;
		Scope<std::thread> m_Thread;
		Scope<SocketData> m_Data;

		friend ServerSocket;
	};
}
