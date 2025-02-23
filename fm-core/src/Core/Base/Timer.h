#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/Time.h"

namespace fm {

	class FM_API Timer {

	public:

		Timer();
		Timer(const Timer& other) = delete;

		void Restart();
		Time GetElapsedTime() const;

	private:

		std::chrono::time_point<std::chrono::steady_clock> m_Timer;
	};
}
