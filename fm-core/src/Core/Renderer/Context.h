#pragma once

#include <string>

#include "Core/Base/Base.h"

namespace fm {

	struct ContextSpecification {

		void* WindowPtr = nullptr;
		bool EnableDebug = true;
		std::string AppName = "";
		std::string EngineName = "";
	};

	class FM_API Context {

	public:

		void BeginFrame();
		void EndFrame();

		inline const ContextSpecification& GetSpecification() const { return m_Specification;  }

		static Ref<Context> Create(const ContextSpecification& spec = ContextSpecification());

	private:

		ContextSpecification m_Specification;
	};
}
