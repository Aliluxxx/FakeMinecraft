#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Log.h"

#ifdef FM_ENABLE_ASSERTS
	#define FM_ASSERT(x, ...)          if(!(x)) { FM_ERROR     ("Assertion Failed: {0}", __VA_ARGS__); FM_DEBUGBREAK(); }
	#ifdef FM_ENGINE
		#define FM_CORE_ASSERT(x, ...) if(!(x)) { FM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); FM_DEBUGBREAK(); }
	#endif
#else
	#define FM_ASSERT(x, ...) 0
	#ifdef FM_ENGINE
		#define FM_CORE_ASSERT(x, ...) 0
	#endif
#endif
