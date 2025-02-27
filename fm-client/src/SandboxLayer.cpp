#include <iostream>

#include "SandboxLayer.h"

static void Connect(fm::Socket* socket) {

	fm::IpAddress address = fm::IpAddress::Localhost;//fm::IpAddress("79.45.129.43");//fm::IpAddress::Localhost;79.45.129.43
	fm::Uint16 port = 25565;
	if (socket->Connect(address, port) == fm::Socket::Status::Done) {

		FM_INFO("Connected to {0}:{1}", address.ToString(), port);
	}

	else {

		FM_INFO("Failed to connect to {0}:{1}", address.ToString(), port);
	}
}

void SandboxLayer::OnAttach() {

	Connect(&m_Socket);
}

void SandboxLayer::OnDetach() {

	m_Socket.Disconnect();
}

void SandboxLayer::OnUpdate(fm::Time ts) {

	std::string in;
	std::getline(std::cin, in);

	if (in.compare("/close") == 0) {

		fm::Application::Close();
		return;
	}

	else if (in.compare("/connect") == 0) {

		Connect(&m_Socket);
	}

	else if (in.compare("/disconnect") == 0) {

		m_Socket.Disconnect();
	}

	else {

		if (m_Socket.IsConnected()) {

			fm::Packet packet;
			packet << in;
			fm::Socket::Status status = m_Socket.Send(packet, fm::PacketFlags::Reliable);
			switch (status) {

				case fm::Socket::Status::Done:
					break;
				case fm::Socket::Status::Disconnected:
					FM_INFO("[{0}:{1}] has disconnected", m_Socket.GetRemoteAddress().ToString(), m_Socket.GetRemotePort());
					break;
				case fm::Socket::Status::Timeout:
					FM_WARN("[{0}:{1}] lost connection", m_Socket.GetRemoteAddress().ToString(), m_Socket.GetRemotePort());
					break;
				default:
					FM_ERROR("[{0}:{1}] error", m_Socket.GetRemoteAddress().ToString(), m_Socket.GetRemotePort());
					m_Socket.Disconnect();
					break;
			}
		}
	}
}

void SandboxLayer::OnImGuiRender() {

#ifndef FM_HEADLESS

#endif
}

void SandboxLayer::OnEvent(fm::Event& event) {

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
