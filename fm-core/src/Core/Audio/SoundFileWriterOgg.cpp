#include "fmpch.h"

#include "SoundFileWriterOgg.h"

namespace fm {

	namespace priv {

		SoundFileWriterOgg::SoundFileWriterOgg()
			: m_ChannelCount(0), m_File(), m_Ogg(), m_Vorbis(), m_State()

		{}

		SoundFileWriterOgg::~SoundFileWriterOgg() {

			Close();
		}

		bool SoundFileWriterOgg::Check(const std::filesystem::path& path) {

			return path.extension().string().compare(".ogg") || path.extension().string().compare(".OGG");
		}

		bool SoundFileWriterOgg::Open(const std::filesystem::path& path, Uint32 sample_rate, Uint32 channel_count) {

			m_ChannelCount = channel_count;

			ogg_stream_init(&m_Ogg, std::rand());
			vorbis_info_init(&m_Vorbis);

			int status = vorbis_encode_init_vbr(&m_Vorbis, static_cast<long>(channel_count), static_cast<long>(sample_rate), 0.4f);
			if (status < 0) {

				FM_CORE_ERROR("Failed to write ogg/vorbis file (unsupported bitrate): \"{}\"", path.string());
				Close();

				return false;
			}

			vorbis_analysis_init(&m_State, &m_Vorbis);

			m_File.open(path.c_str(), std::ios::binary);
			if (!m_File) {

				FM_CORE_ERROR("Failed to write ogg/vorbis file (cannot open file): \"{}\"", path.string());
				Close();

				return false;
			}

			vorbis_comment comment;
			vorbis_comment_init(&comment);

			ogg_packet header, headerComm, headerCode;
			status = vorbis_analysis_headerout(&m_State, &comment, &header, &headerComm, &headerCode);
			vorbis_comment_clear(&comment);
			if (status < 0) {

				FM_CORE_ERROR("Failed to write ogg/vorbis file (cannot generate the headers): \"{}\"", path.string());
				Close();

				return false;
			}

			ogg_stream_packetin(&m_Ogg, &header);
			ogg_stream_packetin(&m_Ogg, &headerComm);
			ogg_stream_packetin(&m_Ogg, &headerCode);

			ogg_page page;
			while (ogg_stream_flush(&m_Ogg, &page) > 0) {

				m_File.write(reinterpret_cast<const char*>(page.header), page.header_len);
				m_File.write(reinterpret_cast<const char*>(page.body), page.body_len);
			}

			return true;
		}

		void SoundFileWriterOgg::Write(const Int16* samples, Uint64 count) {

			constexpr int bufferSize = 65536;
			int frameCount = static_cast<int>(count / m_ChannelCount);

			while (frameCount > 0) {

				float** buffer = vorbis_analysis_buffer(&m_State, bufferSize);
				FM_CORE_ASSERT(buffer, "The buffer is NULL");

				for (int i = 0; i < std::min(frameCount, bufferSize); ++i)
					for (unsigned int j = 0; j < m_ChannelCount; ++j)
						buffer[j][i] = *samples++ / 32767.0f;

				vorbis_analysis_wrote(&m_State, std::min(frameCount, bufferSize));
				frameCount -= bufferSize;
				FlushBlocks();
			}
		}

		void SoundFileWriterOgg::FlushBlocks() {

			vorbis_block block;
			vorbis_block_init(&m_State, &block);
			while (vorbis_analysis_blockout(&m_State, &block) == 1) {

				vorbis_analysis(&block, nullptr);
				vorbis_bitrate_addblock(&block);

				ogg_packet packet;
				while (vorbis_bitrate_flushpacket(&m_State, &packet)) {

					ogg_stream_packetin(&m_Ogg, &packet);

					ogg_page page;
					while (ogg_stream_flush(&m_Ogg, &page) > 0) {

						m_File.write(reinterpret_cast<const char*>(page.header), page.header_len);
						m_File.write(reinterpret_cast<const char*>(page.body), page.body_len);
					}
				}
			}

			vorbis_block_clear(&block);
		}

		void SoundFileWriterOgg::Close() {

			if (m_File.is_open()) {

				vorbis_analysis_wrote(&m_State, 0);
				FlushBlocks();

				m_File.close();
			}

			ogg_stream_clear(&m_Ogg);
			vorbis_dsp_clear(&m_State);
			vorbis_info_clear(&m_Vorbis);
		}
	}
}
