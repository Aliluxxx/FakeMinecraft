#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"
#include "Core/Base/Vector3.h"

namespace fm {

	struct MicrophoneData;

	class FM_API Microphone {

	public:

		Microphone(const std::string& device);

		static std::vector<std::string> GetAvailableDevices();

		bool Start(Uint32 frequency = 44100, Uint32 channel_count = 1, Uint32 buffer_size = 0);
		void Stop();
		bool IsAvailable() const;
		void SetProcessingInterval(Time interval);
		const std::string& GetDeviceName() const;
		const Uint32& GetChannelCount() const;
		const Uint32& GetSampleRate() const;
		const std::vector<Int16>& GetSamples() const;
		const bool& IsCapturing() const;

	protected:

		virtual bool OnProcessSamples(fm::Int16* samples, std::size_t samples_count) { return true; }

	private:

		Microphone();

		void Record();
		void ProcessCapturedSamples();
		void Cleanup();
		void LaunchCapturingThread();
		void AwaitCapturingThread();

		Ref<MicrophoneData> m_Data;
	};
}
