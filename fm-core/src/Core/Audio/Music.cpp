#include "fmpch.h"

#include "Music.h"

namespace fm {

	Music::Music(const std::filesystem::path& path) {

		Stop();

		if (!m_File.OpenFromFile(path)) {

			FM_CORE_ERROR("Failed to open file: \"{}\"", path.string().c_str());
		}

		Init();
	}

	Music::Music(const void* data, std::size_t size_in_bytes) {

		if (!m_File.OpenFromMemory(data, size_in_bytes)) {

			FM_CORE_ERROR("Failed to open from memory");
		}

		Init();
	}

	Music::Music(InputStream& stream) {

		if (!m_File.OpenFromStream(stream)) {

			FM_CORE_ERROR("Failed to open from stream");
		}

		Init();
	}

	Music::~Music() {

		Stop();
	}

	void Music::SetLoopPoints(TimeSpan time_points) {

		Span<Uint64> samplePoints(TimeToSamples(time_points.Offset), TimeToSamples(time_points.Length));

		if (GetChannelCount() == 0 || m_File.GetSampleCount() == 0) {

			FM_CORE_ERROR("Music is not in a valid state to assign Loop Points");

			return;
		}

		samplePoints.Offset += (static_cast<unsigned long long>(GetChannelCount()) - 1);
		samplePoints.Offset -= (samplePoints.Offset % GetChannelCount());
		samplePoints.Length += (static_cast<unsigned long long>(GetChannelCount()) - 1);
		samplePoints.Length -= (samplePoints.Length % GetChannelCount());

		if (samplePoints.Offset >= m_File.GetSampleCount()) {

			FM_CORE_ERROR("LoopPoints offset val must be in range [0, Duration)");

			return;
		}

		if (samplePoints.Length == 0) {

			FM_CORE_ERROR("LoopPoints length val must be nonzero");

			return;
		}

		samplePoints.Length = std::min(samplePoints.Length, m_File.GetSampleCount() - samplePoints.Offset);

		if (samplePoints.Offset == m_LoopSpan.Offset && samplePoints.Length == m_LoopSpan.Length)
			return;

		Status oldStatus = GetStatus();
		Time oldPos = GetPlayingOffset();

		Stop();

		m_LoopSpan = samplePoints;

		if (oldPos != Time::Zero)
			SetPlayingOffset(oldPos);

		if (oldStatus == Playing)
			Play();
	}

	Music::TimeSpan Music::GetLoopPoints() const {

		return TimeSpan(SamplesToTime(m_LoopSpan.Offset), SamplesToTime(m_LoopSpan.Length));
	}

	bool Music::OnGetData(Chunk& data) {

		std::scoped_lock lock(m_Mutex);

		std::size_t toFill = m_Samples.size();
		Uint64 currentOffset = m_File.GetSampleOffset();
		Uint64 loopEnd = m_LoopSpan.Offset + m_LoopSpan.Length;

		if (IsLooped() && (m_LoopSpan.Length != 0) && (currentOffset <= loopEnd) && (currentOffset + toFill > loopEnd))
			toFill = static_cast<std::size_t>(loopEnd - currentOffset);

		data.Samples = m_Samples.data();
		data.SampleCount = static_cast<std::size_t>(m_File.Read(m_Samples.data(), toFill));
		currentOffset += data.SampleCount;

		return (data.SampleCount != 0) && (currentOffset < m_File.GetSampleCount()) &&
			!(currentOffset == loopEnd && m_LoopSpan.Length != 0);
	}

	void Music::OnSeek(Time time_offset) {

		std::scoped_lock lock(m_Mutex);

		m_File.Seek(time_offset);
	}

	Int64 Music::OnLoop() {

		std::scoped_lock lock(m_Mutex);

		Uint64 currentOffset = m_File.GetSampleOffset();
		if (IsLooped() && (m_LoopSpan.Length != 0) && (currentOffset == m_LoopSpan.Offset + m_LoopSpan.Length)) {

			m_File.Seek(m_LoopSpan.Offset);

			return static_cast<Int64>(m_File.GetSampleOffset());
		}

		else if (IsLooped() && (currentOffset >= m_File.GetSampleCount())) {

			m_File.Seek(0);

			return 0;
		}

		return NoLoop;
	}

	void Music::Init() {

		m_LoopSpan.Offset = 0;
		m_LoopSpan.Length = m_File.GetSampleCount();
		m_Samples.resize(static_cast<std::size_t>(m_File.GetSampleRate()) * static_cast<std::size_t>(m_File.GetChannelCount()));
		SoundStream::Init(m_File.GetChannelCount(), m_File.GetSampleRate());
	}

	Uint64 Music::TimeToSamples(Time position) const {

		return ((static_cast<Uint64>(position.AsMicroseconds()) * GetSampleRate() * GetChannelCount()) + 500000) / 1000000;
	}

	Time Music::SamplesToTime(Uint64 samples) const {

		Time position = Time::Zero;

		if (GetSampleRate() != 0 && GetChannelCount() != 0)
			position = Microseconds(static_cast<Int64>((samples * 1000000) / (static_cast<unsigned long long>(GetChannelCount()) * GetSampleRate())));

		return position;
	}
}
