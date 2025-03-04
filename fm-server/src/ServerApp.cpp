#include <Core.h>

// ---------- Entry point ----------
#include <Core/Base/EntryPoint.h>
// ---------------------------------

#include "ServerLayer.h"

class ServerApp : public fm::Application {

public:

	ServerApp(const fm::ApplicationSpecification& spec)
		: fm::Application(spec)

	{

		PushLayer(fm::CreateRef<ServerLayer>());
	}

	~ServerApp() {}
};

fm::Application* fm::CreateApplication(fm::ApplicationCommandLineArgs args) {

	ApplicationSpecification spec;
	spec.Name = "FakeMinecraft";
	spec.WorkingDirectory = ".";
	spec.CommandLineArgs = args;
#ifdef FM_HEADLESS
	spec.SleepDuration = fm::Milliseconds(1);
#endif

	return new ServerApp(spec);
}
