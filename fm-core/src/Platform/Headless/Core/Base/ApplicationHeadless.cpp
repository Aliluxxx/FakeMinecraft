#include "fmpch.h"

#include "Core/Base/Sleep.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Renderer/Context.h"
#include "Core/Renderer/Renderer.h"

#include "ApplicationHeadless.h"

namespace fm {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& spec)
		: m_Specification(spec)

	{

		FM_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

#ifdef FM_PLATFORM_WINDOWS
		system(("TITLE " + m_Specification.Name).c_str());
#endif
	}

	Application::~Application() {

		m_LayerStack.Clear();
	}

	void Application::OnEvent(Event& e) {

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {

			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Ref<Layer> layer) {

		s_Instance->m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Ref<Layer> overlay) {

		s_Instance->m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Close() {

		s_Instance->m_Running = false;
	}

	void Application::Run() {

		while (m_Running) {

			float time = m_Timer.GetElapsedTime().AsSeconds();
			Time m_Timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			AppTickEvent e;
			OnEvent(e);

			for (Ref<Layer> layer : m_LayerStack)
				layer->OnUpdate(m_Timestep);

			fm::Sleep(m_Specification.SleepDuration);
		}
	}

	const char* ApplicationCommandLineArgs::operator[](int index) const {

		FM_CORE_ASSERT(index < Count, "Array index out of bounds");
		return Args[index];
	}
}
