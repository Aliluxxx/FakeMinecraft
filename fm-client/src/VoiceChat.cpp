#include "VoiceChat.h"

constexpr std::size_t SampleRate = 44100;
constexpr std::size_t ChannelCount = 1;
constexpr std::size_t MaxBufferSize = SampleRate * ChannelCount;
constexpr std::size_t MinReadableDataSize = 441;

// VoiceChat

VoiceChat::VoiceChat(fm::Ref<fm::Socket> socket)
	:
	fm::SoundStream(),
	m_Mic(socket),
	m_Buffer(MaxBufferSize),
	m_Samples(MaxBufferSize)

{

	Init(ChannelCount, SampleRate);
	SetProcessingInterval(fm::Milliseconds(1));
	m_Mic.SetProcessingInterval(fm::Milliseconds(1));
	m_Mic.Start(SampleRate, ChannelCount);
	SetPitch(1.0f);
	Play();
}

VoiceChat::~VoiceChat() {

	m_Buffer.Terminate();
	Stop();
	m_Mic.Stop();
}

void VoiceChat::LoadFromSamples(fm::Int16* samples, std::size_t samples_count) {

	m_Buffer.Write(samples, samples_count);
}

bool VoiceChat::OnGetData(Chunk& data) {

	data.Samples = m_Samples.data();
	data.SampleCount = m_Buffer.Read(m_Samples.data());

	return true;
}

// MicrophoneStreamer

VoiceChat::MicrophoneStreamer::MicrophoneStreamer(fm::Ref<fm::Socket> socket)
	: fm::Microphone(fm::Microphone::GetAvailableDevices().at(0)), m_Socket(socket)

{

	m_ID = time(NULL);
	FM_INFO("Input Device: {}", GetDeviceName());
	FM_INFO("Output Device: {}", fm::Audio::GetCurrentDevice());
}

bool VoiceChat::MicrophoneStreamer::OnProcessSamples(fm::Int16* samples, std::size_t samples_count) {

	if (!m_Socket->IsConnected())
		return false;

	fm::Packet packet;
	packet << m_ID;
	packet.Append(samples, samples_count * sizeof(fm::Int16));
	fm::Socket::Status status = m_Socket->Send(packet, fm::PacketFlags::Unreliable | fm::PacketFlags::Unsequenced);
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

// Buffer

VoiceChat::Buffer::Buffer(std::size_t size)
	: m_Mutex(),
	m_Buffer(size),
	m_ReadNotifier(),
	m_Size(0)

{}

VoiceChat::Buffer::~Buffer() {

	Terminate();
}

void VoiceChat::Buffer::Terminate() {

	m_ReadNotifier.notify_all();
	m_WriteNotifier.notify_all();
}

void VoiceChat::Buffer::Write(fm::Int16* data, std::size_t size) {

	std::unique_lock<std::recursive_mutex> lock(m_Mutex);

	if (m_Size >= MinReadableDataSize) {

		m_ReadNotifier.notify_all();
		m_WriteNotifier.wait(lock);
	}

	if (m_Size >= MinReadableDataSize)
		return;

	std::memcpy(m_Buffer.data() + m_Size, data, size * sizeof(fm::Int16));
	m_Size += size;
}

std::size_t VoiceChat::Buffer::Read(fm::Int16* data) {

	std::unique_lock<std::recursive_mutex> lock(m_Mutex);

	if (m_Size == 0) {

		m_WriteNotifier.notify_all();
		m_ReadNotifier.wait(lock);
	}

	if (m_Size == 0)
		return 0;

	std::memcpy(data, m_Buffer.data(), m_Size * sizeof(fm::Int16));
	std::size_t size = m_Size;
	m_Size = 0;

	return size;
}
