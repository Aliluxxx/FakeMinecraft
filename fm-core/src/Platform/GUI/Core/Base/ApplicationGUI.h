#pragma once

#include <string>
#include <filesystem>

#include "Core/Base/Base.h"
#include "Core/Base/LayerStack.h"
#include "Core/Events/ApplicationEvent.h"
#include "Core/Base/Window.h"
#include "Core/Base/Timer.h"

#ifdef FM_PLATFORM_WINDOWS
	int main(int argc, char** argv);
#else
	int main();
#endif

namespace fm {

	struct FM_API ApplicationCommandLineArgs {

		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const;
	};

	struct FM_API ApplicationSpecification {

		std::string Name = "FakeMinecraft";
		std::filesystem::path WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class FM_API Application {

	public:

		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		void OnEvent(Event& e);

		static void PushLayer(Ref<Layer> layer);
		static void PushOverlay(Ref<Layer> overlay);

		static void Close();

		inline static const Ref<Window> GetWindow() {

			return s_Instance->m_Window;
		}

		inline static const ApplicationSpecification& GetSpecification() {

			return s_Instance->m_Specification;
		}

	private:

		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		static Application* s_Instance;

		ApplicationSpecification m_Specification;
		Ref<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		Timer m_Timer;

#ifdef FM_PLATFORM_WINDOWS
		friend int ::main(int argc, char** argv);
#else
		friend int ::main();
#endif
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}
