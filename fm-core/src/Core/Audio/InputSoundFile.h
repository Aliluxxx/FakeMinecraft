#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class Time;
	class InputStream;
	class SoundFileReader;

	class FM_API InputSoundFile {

	public:

		InputSoundFile();
		InputSoundFile(const InputSoundFile&) = delete;
		InputSoundFile& operator=(const InputSoundFile&) = delete;
		~InputSoundFile();

		bool OpenFromFile(const std::filesystem::path& path);
		bool OpenFromMemory(const void* data, std::size_t size_in_bytes);
		bool OpenFromStream(InputStream& stream);
		void Seek(Uint64 sample_offset);
		void Seek(Time time_offset);
		Uint64 Read(Int16* samples, Uint64 max_count);
		void Close();
		Time GetDuration() const;
		Time GetTimeOffset() const;

		inline const Uint64& GetSampleOffset() const {

			return m_SampleOffset;
		}

		inline const Uint64& GetSampleCount() const {

			return m_SampleCount;
		}

		inline const Uint32& GetSampleRate() const {

			return m_SampleRate;
		}

		inline const Uint32& GetChannelCount() const {

			return m_ChannelCount;
		}

	private:

		struct StreamDeleter {

			StreamDeleter(bool the_owned);

			template<typename T>
			StreamDeleter(const std::default_delete<T>&);

			void operator()(InputStream* ptr) const;

			bool Owned;
		};

		std::unique_ptr<SoundFileReader> m_Reader;
		std::unique_ptr<InputStream, StreamDeleter> m_Stream;
		Uint64 m_SampleOffset;
		Uint64 m_SampleCount;
		Uint32 m_SampleRate;
		Uint32 m_ChannelCount;
	};
}
