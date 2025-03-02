#include <iostream>
#include <thread>

#include "SandboxLayer.h"

static fm::IpAddress g_Address = fm::IpAddress("79.45.129.43");//fm::IpAddress("79.45.129.43");//fm::IpAddress("79.45.129.43");
static fm::Scope<std::thread> g_Thread;

static void Receive(fm::Socket* socket) {

	fm::Socket::Status status = {};
	do {

		fm::Packet packet;

		status = socket->Receive(&packet);

		switch (status) {

			case fm::Socket::Status::Done: {

				std::string s;
				if (packet >> s) {
					FM_INFO("[{0}:{1}]: {2}", socket->GetRemoteAddress().ToString(), socket->GetRemotePort(), s);
				}
				break;
			}

			case fm::Socket::Status::Disconnected:
				FM_INFO("[{0}:{1}] has disconnected", socket->GetRemoteAddress().ToString(), socket->GetRemotePort());
				return;
			case fm::Socket::Status::Timeout:
				FM_WARN("[{0}:{1}] lost connection", socket->GetRemoteAddress().ToString(), socket->GetRemotePort());
				return;
			default:
				FM_ERROR("[{0}:{1}] error", socket->GetRemoteAddress().ToString(), socket->GetRemotePort());
				return;
		}

	} while (status == fm::Socket::Status::Done);
}

static void Connect(fm::Socket* socket) {

	fm::IpAddress address = g_Address;
	fm::Uint16 port = 25565;
	if (socket->Connect(address, port) == fm::Socket::Status::Done) {

		FM_INFO("Connected to {0}:{1}", address.ToString(), port);
		if (g_Thread && g_Thread->joinable())
			g_Thread->join();

		g_Thread = fm::CreateScope<std::thread>(&Receive, socket);
	}

	else {

		FM_INFO("Failed to connect to {0}:{1}", address.ToString(), port);
	}
}

void SandboxLayer::OnAttach() {

	FM_INFO("Public IP: {}", fm::IpAddress::GetPublicAddress().ToString());

	Connect(&m_Socket);
}

void SandboxLayer::OnDetach() {

	m_Socket.Disconnect();
	if (g_Thread && g_Thread->joinable())
		g_Thread->join();
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
		if (g_Thread && g_Thread->joinable())
			g_Thread->join();
	}

	else if (in.compare("/ping") == 0) {

		fm::Uint32 ping = m_Socket.Ping(g_Address, 25565).AsMilliseconds();
		FM_INFO("{}ms", ping);
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

		else {

			FM_WARN("Not connected");
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
