#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class InputStream;

	class FM_API SoundFileReader {

	public:

		struct Info {

			Uint64 SampleCount;
			unsigned int ChannelCount;
			unsigned int SampleRate;
		};

		virtual ~SoundFileReader() {}

		virtual bool Open(InputStream& stream, Info& info) = 0;
		virtual void Seek(Uint64 sample_offset) = 0;
		virtual Uint64 Read(Int16* samples, Uint64 max_count) = 0;
	};
}
