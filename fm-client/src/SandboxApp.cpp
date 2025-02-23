#include <Core.h>

// ---------- Entry point ----------
#include <Core/Base/EntryPoint.h>
// ---------------------------------

#include "SandboxLayer.h"

class Sandbox : public fm::Application {

public:

	Sandbox(const fm::ApplicationSpecification& spec)
		: fm::Application(spec)

	{

		PushLayer(fm::CreateRef<SandboxLayer>());
	}

	~Sandbox() {}
};

fm::Application* fm::CreateApplication(fm::ApplicationCommandLineArgs args) {

	ApplicationSpecification spec;
	spec.Name = "FakeMinecraft";
	spec.WorkingDirectory = ".";
	spec.CommandLineArgs = args;
#ifdef FM_HEADLESS
	spec.SleepDuration = fm::Milliseconds(1);
#endif

	return new Sandbox(spec);
}
