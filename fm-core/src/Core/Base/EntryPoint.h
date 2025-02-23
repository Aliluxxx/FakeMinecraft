#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Application.h"

extern fm::Application* fm::CreateApplication(ApplicationCommandLineArgs args);

#ifdef FM_PLATFORM_WINDOWS

int main(int argc, char** argv) {

	fm::Log::Init();

	fm::Application* app = fm::CreateApplication({ argc, argv });
	app->Run();
	delete app;

	return 0;
}

#else

int main() {

	fm::Log::Init();

	fm::Application app = fm::CreateApplication({ 0, nullptr });
	app->Run();
	delete app;

	return 0;
}

#endif
