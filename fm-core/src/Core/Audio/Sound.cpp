#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Core/Audio/SoundBuffer.h"

#include "Sound.h"

namespace fm {

	Sound::Sound(SoundBuffer* buffer)
		: m_Buffer(buffer)

	{

		SetBuffer(buffer);
	}

	Sound::Sound()
		: m_Buffer(nullptr)

	{}

	void Sound::Play() {

		alSourcePlay(m_Source);
	}

	void Sound::Pause() {

		alSourcePause(m_Source);
	}

	void Sound::Stop() {

		alSourceStop(m_Source);
	}

	void Sound::ResetBuffer() {

		Stop();

		if (m_Buffer) {

			alSourcei(m_Source, AL_BUFFER, 0);
			m_Buffer = nullptr;
		}
	}

	void Sound::SetBuffer(SoundBuffer* buffer) {

		if (m_Buffer) {

			Stop();
		}

		m_Buffer = buffer;
		alSourcei(m_Source, AL_BUFFER, static_cast<ALint>(m_Buffer->m_Buffer));
	}

	void Sound::SetLoop(bool loop) {

		alSourcei(m_Source, AL_LOOPING, loop);
	}

	void Sound::SetPlayingOffset(const Time& time_offset) {

		alSourcef(m_Source, AL_SEC_OFFSET, time_offset.AsSeconds());
	}

	bool Sound::IsLooped() const {

		ALint loop;
		alGetSourcei(m_Source, AL_LOOPING, &loop);

		return loop != 0;
	}

	Time Sound::GetPlayingOffset() const {

		ALfloat secs = 0.0f;
		alGetSourcef(m_Source, AL_SEC_OFFSET, &secs);

		return Seconds(secs);
	}
}
