#pragma once

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#include "Core/Base/Base.h"
#include "Core/Audio/SoundFileReader.h"

namespace fm {

	namespace priv {

		class FM_API SoundFileReaderOgg : public SoundFileReader {

		public:

			SoundFileReaderOgg();

			~SoundFileReaderOgg() override;

			static bool Check(InputStream& stream);

			bool Open(InputStream& stream, Info& info) override;
			void Seek(Uint64 sample_offset) override;
			Uint64 Read(Int16* samples, Uint64 max_count) override;

		private:

			void Close();

			OggVorbis_File m_Vorbis;
			unsigned int m_ChannelCount;
		};
	}
}
