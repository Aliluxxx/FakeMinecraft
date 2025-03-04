#pragma once

#include <Core.h>

class ServerLayer : public fm::Layer {

public:

	ServerLayer()
		: fm::Layer("Sandbox")

	{}

	virtual ~ServerLayer() = default;

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void OnUpdate(fm::Time ts);
	virtual void OnImGuiRender();
	virtual void OnEvent(fm::Event& event);

private:

	fm::ServerSocket m_ServerSocket;
};
