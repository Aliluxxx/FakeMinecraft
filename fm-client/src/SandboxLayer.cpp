#include <iostream>
#include <thread>

#include "SandboxLayer.h"

static fm::IpAddress g_Address = fm::IpAddress::Localhost;//fm::IpAddress("95.238.73.166");//fm::IpAddress("79.45.129.43");
static fm::Scope<std::thread> g_Thread;

static void Receive(fm::Socket* socket);

static void Connect(fm::Socket* socket) {

	fm::IpAddress address = g_Address;
	fm::Uint16 port = 25565;
	if (socket->Connect(address, port, fm::Milliseconds(5000)) == fm::Socket::Status::Done) {

		FM_INFO("Connected to {0}:{1}", address.ToString(), port);
		if (g_Thread && g_Thread->joinable())
			g_Thread->join();

		g_Thread = fm::CreateScope<std::thread>(&Receive, socket);
	}

	else {

		FM_INFO("Failed to connect to {0}:{1}", address.ToString(), port);
	}
}

constexpr std::size_t MaxSampleCount = 441;

class Buffer {

public:

	Buffer(std::size_t size)
		: m_Mutex(), m_Buffer(size), m_ReadNotifier(), m_Offset(0), m_Size(0)

	{}

	std::size_t GetMaxSize() {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		return m_Buffer.size();
	}

	std::size_t GetSize() {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		return m_Size;
	}

	void Add(const fm::Int16* data, std::size_t size) {

		std::lock_guard<std::recursive_mutex> lock(m_Mutex);

		//if (m_Size + size > m_Buffer.size() - m_SafetySize) {

		//	std::memcpy(m_Buffer.data() + m_Size, data, size * sizeof(fm::Int16));
		//	m_Size = 0;
		//}

		//else {

		//	std::memcpy(m_Buffer.data() + m_Size, data, size * sizeof(fm::Int16));
		//	m_Size += size;
		//}

		//while (m_DataToRead != 0) {

		//	fm::Sleep(fm::Milliseconds(1));
		//}

		//FM_INFO("{0} - {1}", size + m_Size, m_Buffer.size());
		if (size + m_Size > m_Buffer.size()) {

			std::memcpy(m_Buffer.data() + m_Size, data, (m_Buffer.size() - m_Size) * sizeof(fm::Int16));
			std::memcpy(m_Buffer.data(), data, (size - (m_Buffer.size() - m_Size)) * sizeof(fm::Int16));
			//buffer.LoadFromSamples(m_Buffer.data(), m_Buffer.size(), 1, 44100);
			//buffer.SaveToFile("resources/save8.ogg");
			m_Size = size - (m_Buffer.size() - m_Size);
			//FM_INFO("Size exceeded: {}", m_Size);
			FM_WARN("Buffer exceeded");
		}

		else {

			std::memcpy(m_Buffer.data() + m_Size, data, size * sizeof(fm::Int16));

			if (m_Size + size >= MaxSampleCount) {

				//FM_INFO("Available data: {}", size);
				//std::memcpy(m_Buffer.data() + m_Size, data, (MaxSampleCount - m_Size) * sizeof(fm::Int16));

				//std::memcpy(m_ReadBuffer.data(), m_Buffer.data(), MaxSampleCount * sizeof(fm::Int16));
				m_DataToRead = m_Size + size;
				m_Size = m_DataToRead - MaxSampleCount;
				m_ReadNotifier.notify_all();
				//std::this_thread::yield();
				//FM_WARN("Shi");
			}

			else {

				m_Size += size;
			}
		}
	}

	std::size_t Read(fm::Int16* data) {

		std::unique_lock<std::recursive_mutex> lock(m_Mutex);

		while (m_DataToRead == 0)
			m_ReadNotifier.wait(lock);

		std::memcpy(data, m_Buffer.data(), MaxSampleCount * sizeof(fm::Int16));
		std::memmove(m_Buffer.data(), m_Buffer.data() + MaxSampleCount, (m_DataToRead - MaxSampleCount) * sizeof(fm::Int16));
		//FM_INFO("Available samples: {}", m_DataToRead - m_Size);
		//FM_INFO("ka");

		//m_Size = m_DataToRead - MaxSampleCount;
		m_DataToRead = 0;

		//if (m_Offset + size > m_Buffer.size()) {

		//	std::memcpy(data, m_Buffer.data() + m_Offset, (m_Buffer.size() - m_Offset) * sizeof(fm::Int16));
		//	std::memcpy(data, m_Buffer.data(), (size - (m_Buffer.size() - m_Size)) * sizeof(fm::Int16));
		//	m_Offset = 0;
		//}

		//else {

		//	//size = (size > m_Size ? m_Size : size);
		//	//FM_WARN("Read: {}", size);
		//	std::memcpy(data, m_Buffer.data() + m_Offset, size * sizeof(fm::Int16));
		//	m_Offset = (m_Offset + size) % m_Buffer.size();
		//}

		return MaxSampleCount;
	}

private:

	std::recursive_mutex m_Mutex;
	std::vector<fm::Int16> m_Buffer;
	std::condition_variable_any m_ReadNotifier;
	std::size_t m_Offset;
	std::size_t m_Size;
	std::size_t m_DataToRead = 0;
};

fm::Uint64 s_ID;
class MicrophoneStreamer : public fm::Microphone {

public:

	MicrophoneStreamer(fm::Socket* socket)
		: fm::Microphone(fm::Microphone::GetAvailableDevices().at(0)), m_Socket(socket)

	{

		s_ID = time(NULL);
		FM_INFO("Input Device: {}", GetDeviceName());
		FM_INFO("Output Device: {}", fm::Audio::GetCurrentDevice());
	}

	virtual bool OnProcessSamples(fm::Int16* samples, std::size_t samples_count) override {

		fm::Packet packet;
		packet << s_ID;
		packet.Append(samples, samples_count * sizeof(fm::Int16));
		fm::Socket::Status status = m_Socket->Send(packet, fm::PacketFlags::Unreliable);
		switch (status) {

			case fm::Socket::Status::Done:
				break;
			case fm::Socket::Status::Disconnected:
				FM_INFO("[{0}:{1}] has disconnected", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				break;
			case fm::Socket::Status::Timeout:
				FM_WARN("[{0}:{1}] lost connection", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				break;
			default:
				FM_ERROR("[{0}:{1}] error", m_Socket->GetRemoteAddress().ToString(), m_Socket->GetRemotePort());
				m_Socket->Disconnect();
				break;
		}

		return false;
	}

private:

	fm::Socket* m_Socket;
};

class VoiceChat : public fm::SoundStream {

public:

	VoiceChat(fm::Socket* socket)
		:
		fm::SoundStream(),
		m_Mic(socket),
		m_Samples(MaxSampleCount * 1),
		m_Buffer(MaxSampleCount * 90 * 1)

	{

		Init(1, 44100);
		SetProcessingInterval(fm::Milliseconds(1));
		m_Mic.SetProcessingInterval(fm::Milliseconds(1));
		m_Mic.Start();
		SetPitch(1.0f);
		Play();
	}

	~VoiceChat() {

		Stop();
		m_Mic.Stop();
	}

	void SetSamples(fm::Int16* samples, std::size_t samples_count) {

		m_Buffer.Add(samples, samples_count);
	}

protected:

	virtual bool OnGetData(Chunk& data) override {

		data.Samples = m_Samples.data();
		data.SampleCount = m_Buffer.Read(m_Samples.data());

		return true;
	}

	virtual void OnSeek(fm::Time timeOffset) override {


	}

private:

	MicrophoneStreamer m_Mic;
	std::vector<fm::Int16> m_Samples;
	Buffer m_Buffer;
};

static fm::Scope<VoiceChat> s_VoiceChat;

void Receive(fm::Socket* socket) {

	while (!s_VoiceChat) { fm::Sleep(fm::Milliseconds(1)); }

	static std::size_t s_Counter = 0;

	fm::Socket::Status status = {};
	do {

		fm::Packet packet;

		status = socket->Receive(&packet);

		switch (status) {

			case fm::Socket::Status::Done: {

				fm::Uint64 id;
				packet >> id;
				fm::Uint8* data = (fm::Uint8*)packet.GetData();
				if (id != s_ID) {

					s_VoiceChat->SetSamples((fm::Int16*)(data + packet.GetReadPosition()), (packet.GetDataSize() - packet.GetReadPosition()) / 2);
					//FM_INFO("Sample count: {}", (packet.GetDataSize() - packet.GetReadPosition()) / 2);
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

void SandboxLayer::OnAttach() {

	FM_INFO("Public IP: {}", fm::IpAddress::GetPublicAddress().ToString());
	Connect(&m_Socket);
	s_VoiceChat = fm::CreateScope<VoiceChat>(&m_Socket);
}

void SandboxLayer::OnDetach() {

	m_Socket.Disconnect();
	if (g_Thread && g_Thread->joinable())
		g_Thread->join();
}

void SandboxLayer::OnUpdate(fm::Time ts) {


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
