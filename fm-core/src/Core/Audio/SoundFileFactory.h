#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class InputStream;
	class SoundFileReader;
	class SoundFileWriter;

	class FM_API SoundFileFactory {

	public:

		template <typename T>
		static void RegisterReader();
		template <typename T>
		static void UnregisterReader();
		template <typename T>
		static void RegisterWriter();
		template <typename T>
		static void UnregisterWriter();

		static std::unique_ptr<SoundFileReader> CreateReaderFromFilename(const std::filesystem::path& path);
		static std::unique_ptr<SoundFileReader> CreateReaderFromMemory(const void* data, std::size_t size_in_bytes);
		static std::unique_ptr<SoundFileReader> CreateReaderFromStream(InputStream& stream);
		static std::unique_ptr<SoundFileWriter> CreateWriterFromFilename(const std::filesystem::path& path);

	private:

		struct ReaderFactory {

			bool (*Check)(InputStream&);
			std::unique_ptr<SoundFileReader>(*Create)();
		};

		using ReaderFactoryArray = std::vector<ReaderFactory>;

		struct WriterFactory {

			bool (*Check)(const std::filesystem::path&);
			std::unique_ptr<SoundFileWriter>(*Create)();
		};

		using WriterFactoryArray = std::vector<WriterFactory>;

		static ReaderFactoryArray m_Readers;
		static WriterFactoryArray m_Writers;
	};

#include "SoundFileFactory.inl"
}
