#pragma once

#include <mutex>
#include <thread>

#include "Core/Base/Base.h"
#include "Core/Network/Socket.h"

namespace fm {

	struct ServerSocketData;

	class FM_API ServerSocket {

	public:

		ServerSocket(Uint16 port);
		ServerSocket();
		virtual ~ServerSocket();

		Socket::Status Bind(Uint16 port);
		void Unbind();
		bool IsBound() const;
		Socket::Status Accept(Socket* socket);
		Socket::Status Broadcast(const Packet& packet, PacketFlags_ flags = 0, Uint32 channel = 0);

	private:

		void PollEvents();

		mutable std::mutex m_HostMutex;
		mutable std::recursive_mutex m_Mutex;
		Scope<std::thread> m_Thread;
		Scope<ServerSocketData> m_Data;
	};
}
