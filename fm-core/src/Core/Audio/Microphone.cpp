#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Core/Base/Sleep.h"

#include "Microphone.h"

namespace fm {

	struct MicrophoneData {

		std::string Device;
		ALCdevice* alDevice = NULL;
		Scope<std::thread> Thread;
		std::recursive_mutex Mutex;
		std::vector<Int16> Samples;
		Uint32 SampleRate = 44100;
		Time ProcessingInterval = Milliseconds(100);
		bool IsCapturing = false;
		Uint32 ChannelCount = 1;
	};

	std::vector<std::string> Microphone::GetAvailableDevices() {

		char* list;
		if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {

			if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_FALSE)
				list = (char*)(alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER));
			else
				list = (char*)(alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER));

			if (!list)
				return std::vector<std::string>();

			std::vector<std::string> devices;
			ALCchar* ptr = (ALCchar*)list;
			ALCchar* nptr;

			nptr = ptr;
			while (*(nptr += strlen(ptr) + 1) != 0) {

				devices.emplace_back(ptr + 15);
				ptr = nptr;
			}

			devices.emplace_back(ptr + 15);

			return devices;
		}

		FM_CORE_ASSERT(false, "Invalid OpenAL Extension");

		return std::vector<std::string>();
	}

	Microphone::Microphone(const std::string& device) {

		m_Data = CreateRef<MicrophoneData>();
		m_Data->Device = device;
	}

	bool Microphone::Start(Uint32 frequency, Uint32 channel_count, Uint32 buffer_size) {

		m_Data->SampleRate = frequency;
		m_Data->ChannelCount = channel_count;
		ALCenum format = channel_count == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		ALCsizei bufferSize = (ALCsizei)(buffer_size == 0 ? frequency : buffer_size);

		if (!IsAvailable()) {

			FM_CORE_ERROR("Failed to start capture: your system cannot capture audio data (call Microphone::IsAvailable to check it)");

			return false;
		}

		if (m_Data->IsCapturing) {

			FM_CORE_ERROR("Trying to start audio capture, but another capture is already running");

			return false;
		}

		m_Data->Samples.clear();
		m_Data->alDevice = alcCaptureOpenDevice(m_Data->Device.c_str(), frequency, format, bufferSize);

		if (m_Data->alDevice == NULL)
			return false;

		//FM_CORE_ASSERT(m_Data->alDevice, "Failed to start capture (the device does not exist or the specified parameters are not valid)");
		alcCaptureStart(m_Data->alDevice);
		LaunchCapturingThread();

		return true;
	}

	void Microphone::Stop() {

		if (m_Data->IsCapturing)
			AwaitCapturingThread();
	}

	bool Microphone::IsAvailable() const {

		const std::vector<std::string> devices = GetAvailableDevices();
		for (const auto& device : devices)
			if (device.starts_with(m_Data->Device))
				return true;

		return false;
	}

	void Microphone::SetProcessingInterval(Time interval) {

		m_Data->ProcessingInterval = interval;
	}

	const std::string& Microphone::GetDeviceName() const {

		return m_Data->Device;
	}

	const Uint32& Microphone::GetChannelCount() const {

		return m_Data->ChannelCount;
	}

	const Uint32& Microphone::GetSampleRate() const {

		return m_Data->SampleRate;
	}

	const std::vector<Int16>& Microphone::GetSamples() const {

		std::lock_guard<std::recursive_mutex> lock(m_Data->Mutex);

		return m_Data->Samples;
	}

	const bool& Microphone::IsCapturing() const {

		return m_Data->IsCapturing;
	}

	void Microphone::Record() {

		while (m_Data->IsCapturing) {

			ProcessCapturedSamples();
			fm::Sleep(m_Data->ProcessingInterval);
		}

		Cleanup();
	}

	void Microphone::ProcessCapturedSamples() {

		static std::vector<Int16> samples;

		ALCint samplesAvailable;
		alcGetIntegerv(m_Data->alDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALint), &samplesAvailable);

		if (samplesAvailable > 0) {

			std::lock_guard<std::recursive_mutex> lock(m_Data->Mutex);

			samples.resize(static_cast<std::size_t>(samplesAvailable) * GetChannelCount());
			alcCaptureSamples(m_Data->alDevice, samples.data(), samplesAvailable);
			if (OnProcessSamples(samples.data(), samples.size()))
				std::copy(samples.data(), samples.data() + samples.size(), std::back_inserter(m_Data->Samples));
			samples.clear();
		}
	}

	void Microphone::Cleanup() {

		alcCaptureStop(m_Data->alDevice);
		ProcessCapturedSamples();
		alcCaptureCloseDevice(m_Data->alDevice);
		m_Data->alDevice = nullptr;
	}

	void Microphone::LaunchCapturingThread() {

		m_Data->IsCapturing = true;
		FM_CORE_ASSERT(!m_Thread.joinable(), "The thread is not joinable");
		m_Data->Thread = CreateScope<std::thread>(&Microphone::Record, this);
	}

	void Microphone::AwaitCapturingThread() {

		m_Data->IsCapturing = false;

		if (m_Data->Thread && m_Data->Thread->joinable())
			m_Data->Thread->join();
	}
}
