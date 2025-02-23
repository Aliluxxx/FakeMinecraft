#include "fmpch.h"

#include "Time.h"
#ifndef FM_STATIC
const fm::Time fm::Time::Zero;
#endif
#include "Sleep.h"

namespace fm {

	void FM_API Sleep(Time duration) {

		std::this_thread::sleep_for(std::chrono::microseconds(duration.AsMicroseconds()));
	}
}
