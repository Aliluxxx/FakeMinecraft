#pragma once

#include <thread>

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"
#include "Core/Audio/SoundSource.h"

namespace fm {

	class FM_API SoundStream : public SoundSource {

	public:

		struct Chunk {

			const Int16* Samples;
			std::size_t SampleCount;
		};

		~SoundStream() override;

		virtual void Play() override;
		virtual void Pause() override;
		virtual void Stop() override;
		virtual Status GetStatus() const override;

		void SetPlayingOffset(Time timeOffset);
		void SetLoop(bool loop);
		Time GetPlayingOffset() const;

		inline const Uint32& GetSampleRate() const {

			return m_SampleRate;
		}

		inline const Uint32& GetChannelCount() const {

			return m_ChannelCount;
		}

		inline const bool& IsLooped() const {

			return m_Loop;
		}

	protected:

		enum {

			NoLoop = -1
		};

		SoundStream();

		void Init(Uint32 channel_count, Uint32 sample_rate);
		void SetProcessingInterval(Time interval);

		virtual Int64 OnLoop();
		virtual bool OnGetData(Chunk& data) = 0;
		virtual void OnSeek(Time timeOffset) = 0;

	private:

		enum {

			BufferCount = 3,
			BufferRetries = 2
		};

		void StreamData();
		bool FillAndPushBuffer(Uint32 buffer_num, bool immediate_loop = false);
		bool FillQueue();
		void ClearQueue();
		void LaunchStreamingThread(Status thread_start_state);
		void AwaitStreamingThread();

		std::thread m_Thread;
		mutable std::recursive_mutex m_ThreadMutex;
		Status m_ThreadStartState;
		bool m_IsStreaming;
		Uint32 m_Buffers[BufferCount];
		Uint32 m_ChannelCount;
		Uint32 m_SampleRate;
		Int32 m_Format;
		bool m_Loop;
		Uint64 m_SamplesProcessed;
		Int64 m_BufferSeeks[BufferCount];
		Time m_ProcessingInterval;
	};
}
