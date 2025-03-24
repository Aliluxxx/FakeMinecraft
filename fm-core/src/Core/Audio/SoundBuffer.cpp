#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Audio.h"
#include "InputSoundFile.h"
#include "OutputSoundFile.h"

#include "SoundBuffer.h"

namespace fm {

	namespace Utils {

		static Uint32 GetAudioFormatFromChannelCount(Uint32 channel_count) {

			Uint32 format = 0;
			switch (channel_count) {

				case 1:  format = AL_FORMAT_MONO16;                    break;
				case 2:  format = AL_FORMAT_STEREO16;                  break;
				case 4:  format = alGetEnumValue("AL_FORMAT_QUAD16");  break;
				case 6:  format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
				case 7:  format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
				case 8:  format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
				default: format = 0;                                   break;
			}

			// Fixes a bug on OS X
			if (format == -1)
				format = 0;

			return format;
		}
	}

	SoundBuffer::SoundBuffer(const std::filesystem::path& path)
		: m_Buffer(0), m_Samples(), m_Duration(0.0f)

	{

		Audio::Init();
		bool value = LoadFromFile(path);
		FM_CORE_ASSERT(value, "Failed to open file \"{}\"", path.string().c_str());
	}

	SoundBuffer::SoundBuffer(const void* data, std::size_t size_in_bytes)
		: m_Buffer(0), m_Samples(), m_Duration(0.0f)

	{

		Audio::Init();
		bool value = LoadFromMemory(data, size_in_bytes);
		FM_CORE_ASSERT(value, "Failed to read from memory");
	}

	SoundBuffer::SoundBuffer(InputStream& stream)
		: m_Buffer(0), m_Samples(), m_Duration(0.0f)

	{

		Audio::Init();
		bool value = LoadFromStream(stream);
		FM_CORE_ASSERT(value, "Failed to open the stream");
	}

	SoundBuffer::SoundBuffer(const Int16* samples, Uint64 sample_count, Uint32 channel_count, Uint32 sample_rate)
		: m_Buffer(0), m_Samples(), m_Duration(0.0f)

	{

		Audio::Init();
		bool value = LoadFromSamples(samples, sample_count, channel_count, sample_rate);
		FM_CORE_ASSERT(value, "(Failed to load sound buffer from samples (array: &nP={0}, count: {1:d}, channels: {2:d}, samplerate: {3:d})",
			fmt::ptr(samples),
			sample_count,
			channel_count,
			sample_rate);
	}

	SoundBuffer::SoundBuffer()
		: m_Buffer(0), m_Samples(), m_Duration(0.0f)

	{

		Audio::Init();
	}

	SoundBuffer::~SoundBuffer() {

		Destroy();
		Audio::Shutdown();
	}

	bool SoundBuffer::LoadFromFile(const std::filesystem::path& path) {

		alGenBuffers(1, &m_Buffer);

		InputSoundFile file;
		if (file.OpenFromFile(path))
			if (Init(file))
				return true;

		Destroy();

		return false;
	}

	bool SoundBuffer::LoadFromMemory(const void* data, std::size_t size_in_bytes) {

		alGenBuffers(1, &m_Buffer);

		InputSoundFile file;
		if (file.OpenFromMemory(data, size_in_bytes))
			if (Init(file))
				return true;

		Destroy();

		return false;
	}

	bool SoundBuffer::LoadFromStream(InputStream& stream) {

		alGenBuffers(1, &m_Buffer);

		InputSoundFile file;
		if (file.OpenFromStream(stream))
			if (Init(file))
				return true;

		Destroy();

		return false;
	}

	bool SoundBuffer::LoadFromSamples(const Int16* samples, Uint64 sample_count, Uint32 channel_count, Uint32 sample_rate) {

		alGenBuffers(1, &m_Buffer);

		if (samples && sample_count && channel_count && sample_rate) {

			m_Samples.assign(samples, samples + sample_count);
			Update(channel_count, sample_rate);

			return true;
		}

		Destroy();

		return false;
	}

	bool SoundBuffer::SaveToFile(const std::filesystem::path& path) const {

		OutputSoundFile file;
		if (file.OpenFromFile(path, GetSampleRate(), GetChannelCount())) {

			file.Write(m_Samples.data(), m_Samples.size());

			return true;
		}

		else
			return false;
	}

	Uint32 SoundBuffer::GetSampleRate() const {

		ALint sampleRate;
		alGetBufferi(m_Buffer, AL_FREQUENCY, &sampleRate);

		return static_cast<Uint32>(sampleRate);
	}

	Uint32 SoundBuffer::GetChannelCount() const {

		ALint channelCount;
		alGetBufferi(m_Buffer, AL_CHANNELS, &channelCount);

		return static_cast<Uint32>(channelCount);
	}

	bool SoundBuffer::Init(InputSoundFile& file) {

		Uint64 sampleCount = file.GetSampleCount();
		unsigned int channelCount = file.GetChannelCount();
		unsigned int sampleRate = file.GetSampleRate();

		m_Samples.resize(static_cast<std::size_t>(sampleCount));
		if (file.Read(m_Samples.data(), sampleCount) == sampleCount)
			return Update(channelCount, sampleRate);
		else
			return false;
	}

	bool SoundBuffer::Update(Uint32 channel_count, Uint32 sample_rate) {

		if (!channel_count || !sample_rate || m_Samples.empty())
			return false;

		ALenum format = Utils::GetAudioFormatFromChannelCount(channel_count);

		if (format == 0) {

			FM_CORE_ERROR("Failed to load sound buffer (unsupported number of channels: {})", channel_count);

			return false;
		}

		auto size = static_cast<ALsizei>(m_Samples.size() * sizeof(Int16));
		alBufferData(m_Buffer, format, m_Samples.data(), size, static_cast<ALsizei>(sample_rate));

		m_Duration = fm::Seconds(
			static_cast<float>(m_Samples.size()) / static_cast<float>(sample_rate) / static_cast<float>(channel_count));

		return true;
	}

	void SoundBuffer::Destroy() {

		if (m_Buffer != 0)
			alDeleteBuffers(1, &m_Buffer);

		m_Buffer = 0;
	}
}
