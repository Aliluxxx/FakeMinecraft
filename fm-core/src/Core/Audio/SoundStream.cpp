#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Core/Base/Sleep.h"

#include "SoundStream.h"

namespace fm {

	namespace Utils {

		static Uint32 GetAudioFormatFromChannelCount(Uint32 channel_count) {

			Uint32 format = 0;
			switch (channel_count) {

				case 1:  format = AL_FORMAT_MONO16;                    break;
				case 2:  format = AL_FORMAT_STEREO16;                  break;
				case 4:  format = alGetEnumValue("AL_FORMAT_QUAD16");  break;
				case 6:  format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
				case 7:  format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
				case 8:  format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
				default: format = 0;                                   break;
			}

			// Fixes a bug on OS X
			if (format == -1)
				format = 0;

			return format;
		}
	}

	SoundStream::~SoundStream() {

		AwaitStreamingThread();
	}

	void SoundStream::Play() {

		if (m_Format == 0) {

			FM_CORE_ERROR("Failed to play audio stream: sound parameters have not been initialized (call initialize() first)");

			return;
		}

		bool isStreaming = false;
		Status threadStartState = Stopped;

		{
			std::scoped_lock lock(m_ThreadMutex);

			isStreaming = m_IsStreaming;
			threadStartState = m_ThreadStartState;
		}

		if (isStreaming && (threadStartState == Paused)) {

			std::scoped_lock lock(m_ThreadMutex);
			m_ThreadStartState = Playing;
			alSourcePlay(m_Source);

			return;
		}

		else if (isStreaming && (threadStartState == Playing))
			Stop();

		else if (!isStreaming && m_Thread.joinable())
			Stop();

		LaunchStreamingThread(Playing);
	}

	void SoundStream::Pause() {

		{
			std::scoped_lock lock(m_ThreadMutex);

			if (!m_IsStreaming)
				return;

			m_ThreadStartState = Paused;
		}

		alSourcePause(m_Source);
	}

	void SoundStream::Stop() {

		AwaitStreamingThread();
		OnSeek(Time::Zero);
	}

	SoundStream::Status SoundStream::GetStatus() const {

		Status status = SoundSource::GetStatus();

		if (status == Stopped)
		{
			std::scoped_lock lock(m_ThreadMutex);

			if (m_IsStreaming)
				status = m_ThreadStartState;
		}

		return status;
	}

	void SoundStream::SetPlayingOffset(Time timeOffset) {

		Status oldStatus = GetStatus();

		Stop();

		OnSeek(timeOffset);

		m_SamplesProcessed = static_cast<Uint64>(timeOffset.AsSeconds() * static_cast<float>(m_SampleRate)) * m_ChannelCount;

		if (oldStatus == Stopped)
			return;

		LaunchStreamingThread(oldStatus);
	}

	void SoundStream::SetLoop(bool loop) {

		m_Loop = loop;
	}

	Time SoundStream::GetPlayingOffset() const {

		if (m_SampleRate && m_ChannelCount) {

			ALfloat secs = 0.f;
			alGetSourcef(m_Source, AL_SEC_OFFSET, &secs);

			return Seconds(secs + static_cast<float>(m_SamplesProcessed) / static_cast<float>(m_SampleRate) /
				static_cast<float>(m_ChannelCount));
		}

		else
			return Time::Zero;
	}

	SoundStream::SoundStream()
		: m_Thread(), m_ThreadMutex(), m_ThreadStartState(fm::SoundSource::Status::Stopped), m_IsStreaming(false), m_ChannelCount(0),
		m_SampleRate(0), m_Format(0), m_Loop(false), m_SamplesProcessed(0), m_ProcessingInterval(Milliseconds(1))

	{}

	void SoundStream::Init(Uint32 channel_count, Uint32 sample_rate) {

		m_ChannelCount = channel_count;
		m_SampleRate = sample_rate;
		m_SamplesProcessed = 0;

		{
			std::scoped_lock lock(m_ThreadMutex);
			m_IsStreaming = false;
		}

		m_Format = Utils::GetAudioFormatFromChannelCount(channel_count);

		if (m_Format == 0) {

			FM_CORE_ERROR("Unsupported number of channels ({})", m_ChannelCount);

			m_ChannelCount = 0;
			m_SampleRate = 0;
		}
	}

	void SoundStream::SetProcessingInterval(Time interval) {

		m_ProcessingInterval = interval;
	}

	Int64 SoundStream::OnLoop() {

		OnSeek(Time::Zero);

		return 0;
	}

	void SoundStream::StreamData() {

		bool requestStop = false;

		{
			std::scoped_lock lock(m_ThreadMutex);

			// Check if the thread was launched Stopped
			if (m_ThreadStartState == Stopped) {

				m_IsStreaming = false;
				return;
			}
		}

		alGenBuffers(BufferCount, m_Buffers);
		for (Int64& bufferSeek : m_BufferSeeks)
			bufferSeek = NoLoop;

		requestStop = FillQueue();

		alSourcePlay(m_Source);

		{
			std::scoped_lock lock(m_ThreadMutex);

			if (m_ThreadStartState == Paused)
				alSourcePause(m_Source);
		}

		for (;;) {

			{
				std::scoped_lock lock(m_ThreadMutex);
				if (!m_IsStreaming)
					break;
			}

			if (SoundSource::GetStatus() == Stopped) {

				if (!requestStop) {

					alSourcePlay(m_Source);
				}

				else {

					std::scoped_lock lock(m_ThreadMutex);
					m_IsStreaming = false;
				}
			}

			ALint nbProcessed = 0;
			alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &nbProcessed);

			while (nbProcessed--) {

				ALuint buffer;
				alSourceUnqueueBuffers(m_Source, 1, &buffer);

				unsigned int bufferNum = 0;
				for (unsigned int i = 0; i < BufferCount; ++i)
					if (m_Buffers[i] == buffer) {

						bufferNum = i;
						break;
					}

				if (m_BufferSeeks[bufferNum] != NoLoop) {

					m_SamplesProcessed = static_cast<Uint64>(m_BufferSeeks[bufferNum]);
					m_BufferSeeks[bufferNum] = NoLoop;
				}

				else {

					ALint size, bits;
					alGetBufferi(buffer, AL_SIZE, &size);
					alGetBufferi(buffer, AL_BITS, &bits);

					if (bits == 0) {

						FM_CORE_ASSERT(false, "Bits in sound stream are 0: make sure that the audio format is not corrupt and Init() has been called correctly");

						std::scoped_lock lock(m_ThreadMutex);
						m_IsStreaming = false;
						requestStop = true;
						break;
					}

					else {

						m_SamplesProcessed += static_cast<Uint64>(size / (bits / 8));
					}
				}

				if (!requestStop) {

					if (FillAndPushBuffer(bufferNum))
						requestStop = true;
				}
			}

			if (alGetError() != AL_NO_ERROR) {

				std::scoped_lock lock(m_ThreadMutex);
				m_IsStreaming = false;
				break;
			}

			if (SoundSource::GetStatus() != Stopped)
				fm::Sleep(m_ProcessingInterval);
		}

		alSourceStop(m_Source);

		ClearQueue();

		m_SamplesProcessed = 0;
		alSourcei(m_Source, AL_BUFFER, 0);
		alDeleteBuffers(BufferCount, m_Buffers);
	}

	bool SoundStream::FillAndPushBuffer(Uint32 buffer_num, bool immediate_loop) {

		bool requestStop = false;

		Chunk data = { nullptr, 0 };
		for (Uint32 retryCount = 0; !OnGetData(data) && (retryCount < BufferRetries); ++retryCount) {

			if (!m_Loop) {

				if (data.Samples != nullptr && data.SampleCount != 0)
					m_BufferSeeks[buffer_num] = 0;

				requestStop = true;

				break;
			}

			m_BufferSeeks[buffer_num] = OnLoop();

			if (data.Samples != nullptr && data.SampleCount != 0)
				break;

			if (immediate_loop && (m_BufferSeeks[buffer_num] != NoLoop)) {

				m_SamplesProcessed = static_cast<Uint64>(m_BufferSeeks[buffer_num]);
				m_BufferSeeks[buffer_num] = NoLoop;
			}
		}

		if (data.Samples && data.SampleCount) {

			unsigned int buffer = m_Buffers[buffer_num];
			auto size = static_cast<ALsizei>(data.SampleCount * sizeof(Int16));
			alBufferData(buffer, m_Format, data.Samples, size, static_cast<ALsizei>(m_SampleRate));
			alSourceQueueBuffers(m_Source, 1, &buffer);
		}

		else
			requestStop = true;

		return requestStop;
	}

	bool SoundStream::FillQueue() {

		bool requestStop = false;
		for (unsigned int i = 0; (i < BufferCount) && !requestStop; ++i) {

			if (FillAndPushBuffer(i, (i == 0)))
				requestStop = true;
		}

		return requestStop;
	}

	void SoundStream::ClearQueue() {

		// Get the number of buffers still in the queue
		ALint nbQueued;
		alGetSourcei(m_Source, AL_BUFFERS_QUEUED, &nbQueued);

		// Dequeue them all
		ALuint buffer;
		for (ALint i = 0; i < nbQueued; ++i)
			alSourceUnqueueBuffers(m_Source, 1, &buffer);
	}

	void SoundStream::LaunchStreamingThread(Status thread_start_state) {

		{
			std::scoped_lock lock(m_ThreadMutex);
			m_IsStreaming = true;
			m_ThreadStartState = thread_start_state;
		}

		FM_CORE_ASSERT(!m_Thread.joinable(), "The thread is not joinable");
		m_Thread = std::thread(&SoundStream::StreamData, this);
	}

	void SoundStream::AwaitStreamingThread() {

		{
			std::scoped_lock lock(m_ThreadMutex);
			m_IsStreaming = false;
		}

		if (m_Thread.joinable())
			m_Thread.join();
	}
}
