#pragma once

#include <string>
#include <filesystem>

#include "Core/Base/Base.h"
#include "Core/Base/LayerStack.h"
#include "Core/Base/Timer.h"

#ifdef FM_PLATFORM_WINDOWS
	int main(int argc, char** argv);
#else
	int main();
#endif

namespace fm {

	struct ApplicationCommandLineArgs {

		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const;
	};

	struct ApplicationSpecification {

		std::string Name = "FakeMinecraft Headless";
		std::filesystem::path WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
		Time SleepDuration;
	};

	class FM_API Application {

	public:

		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		void OnEvent(Event& e);

		static void PushLayer(Ref<Layer> layer);
		static void PushOverlay(Ref<Layer> overlay);

		static void Close();

		static inline const ApplicationSpecification& GetSpecification() {

			return s_Instance->m_Specification;
		}

	private:

		void Run();

		static Application* s_Instance;

		ApplicationSpecification m_Specification;

		bool m_Running = true;
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
