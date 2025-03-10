#pragma once

#include <atomic>

#include "Core/Base/Base.h"
#include "Core/Base/Vector3.h"

namespace fm {

	class SoundSource;
	class SoundBuffer;

	class FM_API Audio {

	public:

		static std::vector<std::string> GetAvailableDevices();
		static void SetNewDevice(const std::string& device);
		static const std::string& GetCurrentDevice();

		static void SetListenerVolume(float volume);
		static void SetListenerPosition(const Vector3f& position);
		static void SetListenerPosition(float x, float y, float z);
		static void SetListenerVelocity(const Vector3f& velocity);
		static void SetListenerVelocity(float x, float y, float z);
		static void SetListenerDirection(const Vector3f& direction);
		static void SetListenerDirection(float x, float y, float z);
		static void SetListenerOrientation(float x1, float y1, float z1, float x2, float y2, float z2);
		static void SetListenerOrientation(const Vector3f& front, const Vector3f& up);

	private:

		static void DestroyOpenALContext();

		static void Init();
		static void Shutdown();

		static std::atomic<Uint32> s_ALInstances;

		friend class SoundSource;
		friend class SoundBuffer;
	};
}
