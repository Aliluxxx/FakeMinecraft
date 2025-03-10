#pragma once

#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

#include "Core/Base/Base.h"
#include "Core/Audio/SoundFileWriter.h"

namespace fm {

	namespace priv {

		class FM_API SoundFileWriterOgg : public SoundFileWriter {

		public:

			SoundFileWriterOgg();
			~SoundFileWriterOgg() override;

			static bool Check(const std::filesystem::path& path);

			virtual bool Open(const std::filesystem::path& path, Uint32 sample_rate, Uint32 channel_count) override;
			virtual void Write(const Int16* samples, Uint64 count) override;

		private:

			void FlushBlocks();
			void Close();

			Uint32 m_ChannelCount;
			std::ofstream m_File;
			ogg_stream_state m_Ogg;
			vorbis_info m_Vorbis;
			vorbis_dsp_state m_State;
		};
	}
}
