#pragma once

#include <filesystem>

#include "Core/Base/Base.h"

namespace fm {

	class FM_API SoundFileWriter {

	public:

		virtual ~SoundFileWriter() {}

		virtual bool Open(const std::filesystem::path& path,
			Uint32 sample_rate,
			Uint32 channel_count) = 0;

		virtual void Write(const Int16* samples, Uint64 count) = 0;
	};
}
