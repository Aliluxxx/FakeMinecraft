#include "fmpch.h"

#include "Timer.h"

namespace fm {

	Timer::Timer() {

		Restart();
	}

	void Timer::Restart() {

		m_Timer = std::chrono::high_resolution_clock::now();
	}

	Time Timer::GetElapsedTime() const {

		return Time(
			static_cast<std::int64_t>(
				(std::chrono::high_resolution_clock::now() - m_Timer).count() / 1000ll
			)
		);
	}
}
