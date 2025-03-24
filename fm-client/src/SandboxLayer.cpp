#include <iostream>

#include "SandboxLayer.h"

static fm::IpAddress s_Address = fm::IpAddress("192.168.1.178");//79.20.156.247

void SandboxLayer::Connect() {

	fm::IpAddress address = s_Address;
	fm::Uint16 port = 25565;

	if (m_Socket == nullptr)
		m_Socket = fm::CreateRef<fm::Socket>();

	if (m_Socket->Connect(address, port, fm::Milliseconds(5000)) == fm::Socket::Status::Done) {

		FM_INFO("Connected to {0}:{1}", address.ToString(), port);
		if (m_ReceiveThread && m_ReceiveThread->joinable())
			m_ReceiveThread->join();

		m_VoiceChat = fm::CreateScope<VoiceChat>(m_Socket);
		m_ReceiveThread = fm::CreateScope<std::thread>(&SandboxLayer::Receive, this);
	}

	else {

		FM_INFO("Failed to connect to {0}:{1}", address.ToString(), port);
	}
}

void SandboxLayer::Receive() {

	fm::Socket::Status status = {};
	do {

		fm::Packet packet;

		status = m_Socket->Receive(&packet);

		switch (status) {

			case fm::Socket::Status::Done: {

				fm::Uint64 id;
				packet >> id;
				fm::Uint8* data = (fm::Uint8*)packet.GetData();
				if (id != m_VoiceChat->GetVoiceChatID()) {

					//FM_INFO("Received: {}", (packet.GetDataSize() - packet.GetReadPosition()) / 2);
					m_VoiceChat->LoadFromSamples((fm::Int16*)(data + packet.GetReadPosition()), (packet.GetDataSize() - packet.GetReadPosition()) / 2);
				}

				break;
			}

			case fm::Socket::Status::Disconnected:
				FM_INFO("[{0}:{1}] has disconnected", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				return;
			case fm::Socket::Status::Timeout:
				FM_WARN("[{0}:{1}] lost connection", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				return;
			default:
				FM_ERROR("[{0}:{1}] error", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				return;
		}

	} while (status == fm::Socket::Status::Done);
}

void SandboxLayer::OnAttach() {

	FM_INFO("Public IP: {}", fm::IpAddress::GetPublicAddress().ToString());

	Connect();
}

void SandboxLayer::OnDetach() {

	m_Socket->Disconnect();
	if (m_ReceiveThread && m_ReceiveThread->joinable())
		m_ReceiveThread->join();
}

void SandboxLayer::OnUpdate(fm::Time ts) {

#ifdef FM_HEADLESS
	std::string in;
	std::getline(std::cin, in);
	if (in.compare("/exit") == 0) {

		fm::Application::Close();
	}
#else
	fm::Sleep(fm::Milliseconds(1));
#endif
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
