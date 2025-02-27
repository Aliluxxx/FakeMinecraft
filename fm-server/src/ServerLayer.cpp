#include <iostream>

#include "ServerLayer.h"

class ClientSkeleton {

public:

	ClientSkeleton(fm::Ref<fm::Socket> socket)
		: m_Socket(socket), m_Thread(&ClientSkeleton::Receive, this)

	{

		FM_INFO("{0}:{1} has connected", socket->GetRemoteAddress().ToString(), socket->GetRemotePort());
	}

	~ClientSkeleton() {

		m_Thread.join();
	}

private:

	void Receive() {

		fm::Socket::Status status = fm::Socket::Status::Done;
		fm::Packet packet;
		std::string s;
		while (status == fm::Socket::Status::Done) {

			status = m_Socket->Receive(&packet);

			switch (status) {

				case fm::Socket::Status::Done:

					if (packet >> s) {

						if (s.starts_with("nigga") ||
							s.starts_with("nigger"))

						{

							packet.Clear();
							packet << "You have been banned";
							status = m_Socket->Send(packet, fm::PacketFlags::Reliable);
							fm::Sleep(fm::Milliseconds(10));
							m_Socket->Disconnect();
							packet.Clear();
						}

						else {

							FM_INFO("[{0}:{1}]: {2}", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort(), s);
						}
					}

					break;
				case fm::Socket::Status::Disconnected:
					FM_INFO("[{0}:{1}] has disconnected", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
					break;
				case fm::Socket::Status::Timeout:
					FM_WARN("[{0}:{1}] lost connection", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
					break;
				default:
					FM_ERROR("[{0}:{1}] error", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
					break;
			}
		}
	}

	fm::Ref<fm::Socket> m_Socket;
	std::thread m_Thread;
};

void ServerLayer::OnAttach() {

	if (m_ServerSocket.Bind(25565) == fm::Socket::Status::Done) {

		FM_INFO("Listening on port 25565");
	}

	else {

		fm::Application::Close();
	}
}

void ServerLayer::OnDetach() {

	m_ServerSocket.Unbind();
}

static std::vector<fm::Ref<ClientSkeleton>> clients;

void ServerLayer::OnUpdate(fm::Time ts) {

	fm::Ref<fm::Socket> socket = fm::CreateRef<fm::Socket>();
	fm::Socket::Status status = m_ServerSocket.Accept(socket.get());

	switch (status) {

		case fm::Socket::Status::Done: {

			fm::Ref<ClientSkeleton> client = fm::CreateRef<ClientSkeleton>(socket);
			clients.push_back(client);
			break;
		}

		case fm::Socket::Status::NotReady:
			FM_INFO("Not Ready");
			break;
		case fm::Socket::Status::Disconnected:
			FM_INFO("Disconnected");
			break;
	}
}

void ServerLayer::OnImGuiRender() {

#ifndef FM_HEADLESS

#endif
}

void ServerLayer::OnEvent(fm::Event& event) {

	fm::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<fm::KeyPressedEvent>([](fm::KeyPressedEvent& event) {

		switch (event.GetKeyCode()) {

			case fm::Key::Escape:
				fm::Application::Close();
				break;
		}

		return false;
	});

	FM_TRACE("{0}", event.ToString());
}
