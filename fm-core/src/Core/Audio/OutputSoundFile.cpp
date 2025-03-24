#include "fmpch.h"

#include "SoundFileFactory.h"
#include "SoundFileWriter.h"

#include "OutputSoundFile.h"

namespace fm {

	OutputSoundFile::OutputSoundFile()

		:

		m_Writer(nullptr)

	{}

	OutputSoundFile::~OutputSoundFile() {

		Close();
	}

	bool OutputSoundFile::OpenFromFile(const std::filesystem::path& path, Uint32 sample_rate, Uint32 channel_count) {

		Close();

		m_Writer = SoundFileFactory::CreateWriterFromFilename(path);
		if (!m_Writer)
			return false;

		if (!m_Writer->Open(path, sample_rate, channel_count)) {

			Close();

			return false;
		}

		return true;
	}

	void OutputSoundFile::Write(const Int16* samples, Uint64 count) {

		if (m_Writer && samples && count)
			m_Writer->Write(samples, count);
	}

	void OutputSoundFile::Close() {

		m_Writer.reset();
	}
}
