#include "SandboxLayer.h"

void SandboxLayer::OnAttach() {


}

void SandboxLayer::OnDetach() {


}

void SandboxLayer::OnUpdate(fm::Time ts) {


}

void SandboxLayer::OnImGuiRender() {

#ifndef FM_HEADLESS

#endif
}

void SandboxLayer::OnEvent(fm::Event& event) {

	fm::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<fm::KeyPressedEvent>([](fm::KeyPressedEvent& event) {

		switch (event.GetKeyCode()) {

			case fm::Key::Escape:
				fm::Application::Close();
				break;
		}

		return false;
	});

	FM_TRACE("{0}", event.ToString());
}
