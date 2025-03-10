#include "fmpch.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "Core/Base/Sleep.h"

#include "Audio.h"

namespace fm {

	struct AudioData {

		ALCdevice* alDevice = NULL;
		ALCcontext* Context = NULL;
		std::string Device;
	};

	static Scope<AudioData> s_Data;

	std::atomic<Uint32> Audio::s_ALInstances = 0;

	void Audio::Init() {

		if (s_ALInstances == 0) {

			s_Data = CreateScope<AudioData>();

			if (!GetAvailableDevices().empty())
				SetNewDevice(GetAvailableDevices().at(0));
			else
				FM_CORE_ASSERT(false);

			s_ALInstances++;
		}

		else
			s_ALInstances++;
	}

	void Audio::Shutdown() {

		if (s_ALInstances > 1)
			s_ALInstances--;

		else {

			s_ALInstances--;
			DestroyOpenALContext();
		}
	}

	void Audio::SetNewDevice(const std::string& device) {

		DestroyOpenALContext();
		s_Data->Device = device;
		s_Data->alDevice = alcOpenDevice(device.c_str());
		s_Data->Context = alcCreateContext(s_Data->alDevice, nullptr);
		alcMakeContextCurrent(s_Data->Context);
	}

	std::vector<std::string> Audio::GetAvailableDevices() {

		char* list;
		if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_TRUE) {

			if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") == AL_FALSE)
				list = (char*)(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
			else
				list = (char*)(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));

			if (!list)
				return std::vector<std::string>();

			std::vector<std::string> devices;
			ALCchar* ptr = (ALCchar*)list;
			ALCchar* nptr;

			nptr = ptr;
			while (*(nptr += strlen(ptr) + 1) != 0) {

				devices.push_back(ptr + 15);
				ptr = nptr;
			}

			devices.push_back(ptr + 15);

			return devices;
		}

		FM_CORE_ASSERT(false, "Invalid OpenAL Extension");

		return std::vector<std::string>();
	}

	const std::string& Audio::GetCurrentDevice() {

		return s_Data->Device;
	}

	void Audio::DestroyOpenALContext() {

		if (s_Data->Context)
			alcDestroyContext(s_Data->Context);
	}

	// Listener

	void Audio::SetListenerVolume(float volume) {

		alListenerf(AL_GAIN, volume);
	}

	void Audio::SetListenerPosition(const Vector3f& position) {

		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void Audio::SetListenerPosition(float x, float y, float z) {

		alListener3f(AL_POSITION, x, y, z);
	}

	void Audio::SetListenerVelocity(const Vector3f& velocity) {

		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void Audio::SetListenerVelocity(float x, float y, float z) {

		alListener3f(AL_VELOCITY, x, y, z);
	}

	void Audio::SetListenerDirection(const Vector3f& direction) {

		alListener3f(AL_DIRECTION, direction.x, direction.y, direction.z);
	}

	void Audio::SetListenerDirection(float x, float y, float z) {

		alListener3f(AL_DIRECTION, x, y, z);
	}

	void Audio::SetListenerOrientation(float x1, float y1, float z1, float x2, float y2, float z2) {

		ALfloat vecs[6] = { x1, y1, z1, x2, y2, z2 };
		alListenerfv(AL_ORIENTATION, vecs);
	}

	void Audio::SetListenerOrientation(const Vector3f& front, const Vector3f& up) {

		ALfloat vecs[6] = { front.x, front.y, front.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, vecs);
	}

	// Listener storage

	/*float Audio::Listener::s_Volume = 100.0f;
	Vector3f Audio::Listener::s_Position(0.0f, 0.0f, 0.0f);
	Vector3f Audio::Listener::s_Direction(0.0f, 0.0f, -1.0f);
	Vector3f Audio::Listener::s_VectorUp(0.0f, 1.0f, 0.0f);*/
}
