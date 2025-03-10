#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "Audio.h"

#include "SoundSource.h"

namespace fm {

	SoundSource::SoundSource()

		:

		m_Source(0)

	{

		Audio::Init();
		alGenSources(1, &m_Source);
	}

	SoundSource::~SoundSource() {

		alDeleteSources(1, &m_Source);
		Audio::Shutdown();
	}

	void SoundSource::SetPitch(float pitch) {

		alSourcef(m_Source, AL_PITCH, pitch);
	}

	void SoundSource::SetVolume(float volume) {

		alSourcef(m_Source, AL_GAIN, volume);
	}

	void SoundSource::SetPosition(const Vector3f& position) {

		alSource3f(m_Source, AL_POSITION, position.x, position.y, position.z);
	}

	void SoundSource::SetPosition(float x, float y, float z) {

		alSource3f(m_Source, AL_POSITION, x, y, z);
	}

	void SoundSource::SetRelativeToListener(bool relative) {

		alSourcei(m_Source, AL_SOURCE_RELATIVE, relative);
	}

	void SoundSource::SetMinDistance(float distance) {

		alSourcef(m_Source, AL_REFERENCE_DISTANCE, distance);
	}

	void SoundSource::SetAttenuation(float attenuation) {

		alSourcef(m_Source, AL_ROLLOFF_FACTOR, attenuation);
	}

	float SoundSource::GetPitch() const {

		ALfloat pitch;
		alGetSourcef(m_Source, AL_PITCH, &pitch);

		return pitch;
	}

	float SoundSource::GetVolume() const {

		ALfloat volume;
		alGetSourcef(m_Source, AL_GAIN, &volume);

		return volume;
	}

	Vector3f SoundSource::GetPosition() const {

		Vector3f position{};
		alGetSource3f(m_Source, AL_POSITION, &position.x, &position.y, &position.z);

		return position;
	}

	bool SoundSource::IsRelativeToListener() const {

		ALint relative;
		alGetSourcei(m_Source, AL_SOURCE_RELATIVE, &relative);

		return relative;
	}

	float SoundSource::GetMinDistance() const {

		ALfloat distance;
		alGetSourcef(m_Source, AL_REFERENCE_DISTANCE, &distance);

		return distance;
	}

	float SoundSource::GetAttenuation() const {

		ALfloat attenuation;
		alGetSourcef(m_Source, AL_ROLLOFF_FACTOR, &attenuation);

		return attenuation;
	}

	SoundSource::Status SoundSource::GetStatus() const {

		ALint status;
		alGetSourcei(m_Source, AL_SOURCE_STATE, &status);

		switch (status) {

			case AL_INITIAL:
			case AL_STOPPED:
				return Stopped;
			case AL_PAUSED:
				return Paused;
			case AL_PLAYING:
				return Playing;
		}

		return Stopped;
	}
}
