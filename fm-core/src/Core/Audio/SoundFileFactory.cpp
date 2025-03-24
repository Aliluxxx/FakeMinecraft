#include "fmpch.h"

#include "Core/Base/FileInputStream.h"
#include "Core/Base/MemoryInputStream.h"
#include "SoundFileReaderOgg.h"
#include "SoundFileWriterOgg.h"
#include "SoundFileFactory.h"

namespace {

	void EnsureDefaultReadersWritersRegistered() {

		static bool registered = false;
		if (!registered) {

			fm::SoundFileFactory::RegisterReader<fm::priv::SoundFileReaderOgg>();
			fm::SoundFileFactory::RegisterWriter<fm::priv::SoundFileWriterOgg>();
			registered = true;
		}
	}
}

namespace fm {

	std::unique_ptr<SoundFileReader> SoundFileFactory::CreateReaderFromFilename(const std::filesystem::path& path) {

		EnsureDefaultReadersWritersRegistered();

		FileInputStream stream;

		if (!stream.Open(path)) {

			FM_CORE_ERROR("Failed to open sound file (could not open stream): \"{}\"", path.string());

			return nullptr;
		}

		for (const ReaderFactory& readerFactory : m_Readers) {

			if (stream.Seek(0) == -1) {

				FM_CORE_ERROR("Failed to seek sound stream");

				return nullptr;
			}

			if (readerFactory.Check(stream))
				return readerFactory.Create();
		}

		FM_CORE_ERROR("Failed to open sound file (format not supported): \"{}\"", path.string());

		return nullptr;
	}

	std::unique_ptr<SoundFileReader> SoundFileFactory::CreateReaderFromMemory(const void* data, std::size_t size_in_bytes) {

		EnsureDefaultReadersWritersRegistered();

		MemoryInputStream stream;
		stream.Open(data, size_in_bytes);

		for (const ReaderFactory& readerFactory : m_Readers) {

			if (stream.Seek(0) == -1) {

				FM_CORE_ERROR("Failed to seek sound stream");

				return nullptr;
			}

			if (readerFactory.Check(stream))
				return readerFactory.Create();
		}

		FM_CORE_ERROR("Failed to open sound file from memory (format not supported)");

		return nullptr;
	}

	std::unique_ptr<SoundFileReader> SoundFileFactory::CreateReaderFromStream(InputStream& stream) {

		EnsureDefaultReadersWritersRegistered();

		for (const ReaderFactory& readerFactory : m_Readers) {

			if (stream.Seek(0) == -1) {

				FM_CORE_ERROR("Failed to seek sound stream");

				return nullptr;
			}

			if (readerFactory.Check(stream))
				return readerFactory.Create();
		}

		FM_CORE_ERROR("Failed to open sound file from stream (format not supported)");

		return nullptr;
	}

	std::unique_ptr<SoundFileWriter> SoundFileFactory::CreateWriterFromFilename(const std::filesystem::path& path) {

		EnsureDefaultReadersWritersRegistered();

		for (const WriterFactory& writerFactory : m_Writers) {

			if (writerFactory.Check(path))
				return writerFactory.Create();
		}

		FM_CORE_ERROR("Failed to open sound file (format not supported)\n{}", path.string());

		return nullptr;
	}

	SoundFileFactory::ReaderFactoryArray SoundFileFactory::m_Readers;
	SoundFileFactory::WriterFactoryArray SoundFileFactory::m_Writers;

}
