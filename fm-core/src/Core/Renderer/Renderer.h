#pragma once

#include "Core/Renderer/Context.h"

namespace fm {

	class FM_API Renderer {

	public:

		static void Init(Ref<Context> context);
		static void Shutdown();

		static void Begin();
		static void End();
	};
}
