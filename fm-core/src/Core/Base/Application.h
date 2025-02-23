#pragma once

#ifdef FM_HEADLESS
	#include "Platform/Headless/Core/Base/ApplicationHeadless.h"
#else
#include "Platform/GUI/Core/Base/ApplicationGUI.h"
#endif
