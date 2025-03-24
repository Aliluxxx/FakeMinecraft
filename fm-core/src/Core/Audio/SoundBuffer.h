#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"

namespace fm {

	class Sound;
	class InputStream;
	class InputSoundFile;

	class FM_API SoundBuffer {

	public:

		SoundBuffer(const std::filesystem::path& path);
		SoundBuffer(const void* data, std::size_t size_in_bytes);
		SoundBuffer(InputStream& stream);
		SoundBuffer(const Int16* samples, Uint64 sample_count, Uint32 channel_count, Uint32 sample_rate);
		SoundBuffer();
		SoundBuffer(const SoundBuffer& other) = delete;
		SoundBuffer& operator=(const SoundBuffer& other) = delete;
		~SoundBuffer();

		bool LoadFromFile(const std::filesystem::path& path);
		bool LoadFromMemory(const void* data, std::size_t size_in_bytes);
		bool LoadFromStream(InputStream& stream);
		bool LoadFromSamples(const Int16* samples, Uint64 sample_count, Uint32 channel_count, Uint32 sample_rate);
		bool SaveToFile(const std::filesystem::path& path) const;

		inline const Int16* GetSamples() const {

			return m_Samples.empty() ? nullptr : m_Samples.data();
		}

		inline Uint64 GetSampleCount() const {

			return (Uint64)m_Samples.size();
		}

		inline const Time& GetDuration() const {

			return m_Duration;
		}

		Uint32 GetSampleRate() const;
		Uint32 GetChannelCount() const;

	private:

		bool Init(InputSoundFile& file);
		bool Update(Uint32 channel_count, Uint32 sample_rate);
		void Destroy();

		Uint32 m_Buffer;
		std::vector<Int16> m_Samples;
		Time m_Duration;

		friend class Sound;
	};
}
