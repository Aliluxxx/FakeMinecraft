#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"
#include "Core/Events/Event.h"

namespace fm {

	class FM_API Layer {

	public:

		Layer(const std::string& debug_name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Time ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

#ifndef FM_DIST
		inline const std::string& GetName() const {

			return m_DebugName;
		}
#else
		inline std::string GetName() const {

			return "";
		}
#endif

	protected:

#ifndef FM_DIST
		std::string m_DebugName;
#endif
	};
}
