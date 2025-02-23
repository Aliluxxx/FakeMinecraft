#include "fmpch.h"

#include "Layer.h"

namespace fm {

#ifdef FM_DIST
	Layer::Layer(const std::string& debug_name) {}
#else
	Layer::Layer(const std::string& debug_name)
		: m_DebugName(debug_name)
	{}
#endif

	Layer::~Layer() {}
}
