#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"
#include "Core/Audio/SoundSource.h"

namespace fm {

	class SoundBuffer;

	class FM_API Sound : public SoundSource {

	public:

		Sound(SoundBuffer* buffer);
		Sound();

		void Play() override;
		void Pause() override;
		void Stop() override;
		void ResetBuffer();
		void SetBuffer(SoundBuffer* buffer);
		void SetLoop(bool loop);
		void SetPlayingOffset(const Time& time_offset);
		bool IsLooped() const;
		Time GetPlayingOffset() const;
		
		Sound::Status GetStatus() const override {
			
			return Sound::Status();
		}

		inline const SoundBuffer* GetSoundBuffer() const {

			return m_Buffer;
		}

	private:

		SoundBuffer* m_Buffer;
	};
}
