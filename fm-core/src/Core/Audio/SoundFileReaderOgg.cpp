#include "fmpch.h"

#include "Core/Base/MemoryInputStream.h"

#include "SoundFileReaderOgg.h"

namespace {

	static std::size_t Read(void* ptr, std::size_t size, std::size_t nmemb, void* data) {

		auto* stream = static_cast<fm::InputStream*>(data);
		return static_cast<std::size_t>(stream->Read(ptr, static_cast<fm::Int64>(size * nmemb)));
	}

	static int Seek(void* data, ogg_int64_t offset, int whence) {

		auto* stream = static_cast<fm::InputStream*>(data);
		switch (whence) {

			case SEEK_SET:
				break;
			case SEEK_CUR:
				offset += stream->Tell();
				break;
			case SEEK_END:
				offset = stream->GetSize() - offset;
		}
		return static_cast<int>(stream->Seek(offset));
	}

	static long Tell(void* data) {

		auto* stream = static_cast<fm::InputStream*>(data);
		return static_cast<long>(stream->Tell());
	}

	static ov_callbacks callbacks = { &Read, &Seek, nullptr, &Tell };
}

namespace fm {

	namespace priv {

		SoundFileReaderOgg::SoundFileReaderOgg()
			: m_Vorbis(), m_ChannelCount(0)

		{

			m_Vorbis.datasource = nullptr;
		}

		SoundFileReaderOgg::~SoundFileReaderOgg() {

			Close();
		}

		bool SoundFileReaderOgg::Check(InputStream& stream) {

			OggVorbis_File file;
			if (ov_test_callbacks(&stream, &file, nullptr, 0, callbacks) == 0) {

				ov_clear(&file);

				return true;
			}

			else
				return false;
		}

		bool SoundFileReaderOgg::Open(InputStream& stream, Info& info) {

			int status = ov_open_callbacks(&stream, &m_Vorbis, nullptr, 0, callbacks);

			if (status < 0) {

				FM_CORE_ERROR("Failed to open Vorbis file for reading");

				return false;
			}

			vorbis_info* vorbisInfo = ov_info(&m_Vorbis, -1);
			info.ChannelCount = static_cast<unsigned int>(vorbisInfo->channels);
			info.SampleRate = static_cast<unsigned int>(vorbisInfo->rate);
			info.SampleCount = static_cast<std::size_t>(ov_pcm_total(&m_Vorbis, -1) * vorbisInfo->channels);

			m_ChannelCount = info.ChannelCount;

			return true;
		}

		void SoundFileReaderOgg::Seek(Uint64 sample_offset) {

			FM_CORE_ASSERT(m_Vorbis.datasource, "The data source is NULL");

			ov_pcm_seek(&m_Vorbis, static_cast<ogg_int64_t>(sample_offset / m_ChannelCount));
		}

		Uint64 SoundFileReaderOgg::Read(Int16* samples, Uint64 max_count) {

			FM_CORE_ASSERT(m_Vorbis.datasource, "The data source is NULL");

			Uint64 count = 0;
			while (count < max_count) {

				int bytesToRead = static_cast<int>(max_count - count) * static_cast<int>(sizeof(Int16));
				int bytesRead = ov_read(&m_Vorbis, reinterpret_cast<char*>(samples), bytesToRead, 0, 2, 1, nullptr);

				if (bytesRead > 0) {

					long samplesRead = bytesRead / static_cast<long>(sizeof(Int16));
					count += static_cast<Uint64>(samplesRead);
					samples += samplesRead;
				}

				else
					break;
			}

			return count;
		}

		void SoundFileReaderOgg::Close() {

			if (m_Vorbis.datasource) {

				ov_clear(&m_Vorbis);
				m_Vorbis.datasource = nullptr;
				m_ChannelCount = 0;
			}
		}
	}
}
