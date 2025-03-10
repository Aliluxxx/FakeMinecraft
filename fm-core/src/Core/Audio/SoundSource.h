#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Vector3.h"

namespace fm {

	class FM_API SoundSource {

	public:

		enum Status {

			Stopped, Paused, Playing
		};

		SoundSource();
		virtual ~SoundSource();

		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;

		void SetPitch(float pitch);
		void SetVolume(float volume);
		void SetPosition(const Vector3f& position);
		void SetPosition(float x, float y, float z);
		void SetRelativeToListener(bool relative);
		void SetMinDistance(float distance);
		void SetAttenuation(float attenuation);
		float GetPitch() const;
		float GetVolume() const;
		Vector3f GetPosition() const;
		bool IsRelativeToListener() const;
		float GetMinDistance() const;
		float GetAttenuation() const;
		virtual Status GetStatus() const;

	protected:

		Uint32 m_Source;
	};
}
