#pragma once

#include <Core.h>

class VoiceChat : public fm::SoundStream {

public:

	VoiceChat(fm::Ref<fm::Socket> socket);
	~VoiceChat();

	void LoadFromSamples(fm::Int16* samples, std::size_t samples_count);

	inline const fm::Uint64& GetVoiceChatID() const {

		return m_Mic.GetID();
	}

protected:

	virtual bool OnGetData(Chunk& data) override;
	virtual void OnSeek(fm::Time timeOffset) override {}

private:

	class MicrophoneStreamer : public fm::Microphone {

	public:

		MicrophoneStreamer(fm::Ref<fm::Socket> socket);
		virtual bool OnProcessSamples(fm::Int16* samples, std::size_t samples_count) override;

		inline const fm::Uint64& GetID() const {

			return m_ID;
		}

	private:

		fm::Uint64 m_ID;
		fm::Ref<fm::Socket> m_Socket;
	};

	class Buffer {

	public:

		Buffer(std::size_t size);
		~Buffer();

		void Terminate();

		inline std::size_t GetMaxSize() {

			std::lock_guard<std::recursive_mutex> lock(m_Mutex);

			return m_Buffer.size();
		}

		inline std::size_t GetSize() {

			std::lock_guard<std::recursive_mutex> lock(m_Mutex);

			return m_Size;
		}

		void Write(fm::Int16* data, std::size_t size);
		std::size_t Read(fm::Int16* data);

	private:

		std::recursive_mutex m_Mutex;
		std::vector<fm::Int16> m_Buffer;
		std::condition_variable_any m_ReadNotifier;
		std::condition_variable_any m_WriteNotifier;
		std::size_t m_Size;
	};

	MicrophoneStreamer m_Mic;
	Buffer m_Buffer;
	std::vector<fm::Int16> m_Samples;
};
