#pragma once

#include "Core/Base/Base.h"
#include "Core/Audio/InputSoundFile.h"
#include "Core/Audio/SoundStream.h"

namespace fm {

	class Time;
	class InputStream;

	class FM_API Music : public SoundStream {

	public:

		template <typename T>
		struct Span {

			Span() {}

			Span(T off, T len) : Offset(off), Length(len) {}

			T Offset;
			T Length;
		};

		// Define the relevant Span types
		using TimeSpan = Span<Time>;

		Music(const std::filesystem::path& path);
		Music(const void* data, std::size_t size_in_bytes);
		Music(InputStream& stream);
		~Music() override;

		void SetLoopPoints(TimeSpan time_points);
		TimeSpan GetLoopPoints() const;

		inline const Time GetDuration() const {

			return m_File.GetDuration();
		}

	protected:

		virtual bool OnGetData(Chunk& data) override;
		virtual void OnSeek(Time time_offset) override;
		virtual Int64 OnLoop() override;

	private:

		void Init();
		Uint64 TimeToSamples(Time position) const;
		Time SamplesToTime(Uint64 samples) const;

		InputSoundFile m_File;
		std::vector<Int16> m_Samples;
		std::recursive_mutex m_Mutex;
		Span<Uint64> m_LoopSpan;
	};
}
