#pragma once

#include <Core.h>

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
};
