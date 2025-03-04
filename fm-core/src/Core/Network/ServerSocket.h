#pragma once

#include <mutex>
#include <thread>

#include "Core/Base/Base.h"
#include "Core/Network/Socket.h"

namespace fm {

	struct ServerSocketData;

	class FM_API ServerSocket {

	public:

		ServerSocket(Uint16 port, Uint32 clients_count = 16, Uint32 channels_count = MINIMUM_CHANNEL_COUNT);
		ServerSocket();
		virtual ~ServerSocket();

		Socket::Status Bind(Uint16 port, Uint32 clients_count = 16, Uint32 channels_count = MINIMUM_CHANNEL_COUNT);
		void Unbind();
		bool IsBound() const;
		Socket::Status Accept(Socket* socket);
		Socket::Status Broadcast(Packet& packet, PacketFlags_ flags = 0, Uint32 channel = PAYLOAD_CHANNEL);

	private:

		void PollEvents();

		mutable std::recursive_mutex m_Mutex;
		Scope<std::thread> m_Thread;
		Scope<ServerSocketData> m_Data;
	};
}
