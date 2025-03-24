#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class Time;
	class SoundFileWriter;

	class FM_API OutputSoundFile {

	public:

		OutputSoundFile();
		OutputSoundFile(const OutputSoundFile&) = delete;
		OutputSoundFile& operator=(const OutputSoundFile&) = delete;
		~OutputSoundFile();

		bool OpenFromFile(const std::filesystem::path& path, Uint32 sample_rate, Uint32 channel_count);
		void Write(const Int16* samples, Uint64 count);
		void Close();

	private:

		std::unique_ptr<SoundFileWriter> m_Writer;
	};
}
