#include "fmpch.h"

#include "FileInputStream.h"

#ifndef FM_PLATFORM_ANDROID
void fm::FileInputStream::FileCloser::operator()(std::FILE* file) {

	std::fclose(file);
}
#endif

namespace fm {

	bool fm::FileInputStream::Open(const std::filesystem::path& path) {

#ifdef FM_PLATFORM_WINDOWS
		std::FILE* tempFile;
		_wfopen_s(&tempFile, path.c_str(), L"rb");
		m_File.reset(tempFile);
#else
		m_File.reset(std::fopen(path.string().c_str(), "rb"));
#endif

		return m_File != nullptr;
	}

	Int64 FileInputStream::Read(void* data, Int64 size) {

		if (m_File)
			return static_cast<Int64>(std::fread(data, 1, static_cast<size_t>(size), m_File.get()));
		else
			return -1;
	}

	Int64 FileInputStream::Seek(Int64 position) {

		if (m_File) {

			if (std::fseek(m_File.get(), static_cast<long>(position), SEEK_SET))
				return -1;

			return Tell();
		}

		else
			return -1;
	}

	Int64 FileInputStream::Tell() {

		if (m_File)
			return std::ftell(m_File.get());
		else
			return -1;
	}

	Int64 FileInputStream::GetSize() {

		if (m_File) {

			Int64 position = Tell();
			std::fseek(m_File.get(), 0, SEEK_END);
			Int64 size = Tell();

			if (Seek(position) == -1)
				return -1;

			return size;
		}

		else
			return -1;
	}
}
