#pragma once

#include <mutex>
#include <atomic>

#include "Core/Base/Base.h"
#include "Core/Network/IpAddress.h"

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

		Socket(IpAddress address, Uint16 port);
		Socket(const Socket& other) = delete;
		Socket();
		virtual ~Socket();

		Status Connect(IpAddress address, Uint16 port);
		bool IsConnected() const;
		Status Send(const Packet& packet, PacketFlags_ flags = 0, Uint32 channel = 0);
		Status Receive(Packet* packet);
		void Disconnect();
		IpAddress GetRemoteAddress() const;
		Uint16 GetRemotePort() const;

	private:

		static void Init();
		static void Shutdown();

		static std::atomic<Uint32> s_ENetInstances;

		void PollEvents();
		void Create(IpAddress address, Uint16 port, void* peer);
		void SetReceivedData(void* packet, std::size_t size);
		void Destroy(Socket::Status status);

		mutable std::recursive_mutex m_Mutex;
		Scope<std::thread> m_Thread;
		Scope<SocketData> m_Data;

		friend ServerSocket;
	};
}
