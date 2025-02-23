#include "fmpch.h"

#include "Core/Renderer/Renderer.h"

#include "Context.h"

namespace fm {

	void Context::BeginFrame() {


	}

	void Context::EndFrame() {


	}

	Ref<Context> Context::Create(const ContextSpecification& spec) {

		return CreateRef<Context>();
	}
}
