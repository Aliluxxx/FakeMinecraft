#pragma once

#include <Core.h>

#include "VoiceChat.h"

class SandboxLayer : public fm::Layer {

public:

	SandboxLayer()
		: fm::Layer("Sandbox")

	{}

	virtual ~SandboxLayer() = default;

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate(fm::Time ts);
	virtual void OnImGuiRender();
	virtual void OnEvent(fm::Event& event);

private:

	void Connect();
	void Receive();

	fm::Ref<fm::Socket> m_Socket;
	fm::Scope<VoiceChat> m_VoiceChat;
	fm::Scope<std::thread> m_ReceiveThread;
};
