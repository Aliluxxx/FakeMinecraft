#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class FM_API Time {

	public:

		constexpr Time() : m_Microseconds(0) {}
		constexpr Time(Int64 micsec) : m_Microseconds(micsec) {}
		constexpr Time(float sec) : m_Microseconds(static_cast<Int64>(sec * 1000000)) {}

		constexpr float AsSeconds() const;
		constexpr Int32 AsMilliseconds() const;
		constexpr Int64 AsMicroseconds() const;
		constexpr Time operator=(Time right);

		static const Time Zero;
		static const Time Infinity;

	private:

		friend constexpr Time Seconds(const float&);
		friend constexpr Time Milliseconds(const Int32&);
		friend constexpr Time Microseconds(const Int64&);

		Int64 m_Microseconds;
	};

	constexpr Time Seconds(const float& amount);
	constexpr Time Milliseconds(const Int32& amount);
	constexpr Time Microseconds(const Int64& amount);

	constexpr bool operator==(Time left, Time right);
	constexpr bool operator!=(Time left, Time right);
	constexpr bool operator<(Time left, Time right);
	constexpr bool operator>(Time left, Time right);
	constexpr bool operator<=(Time left, Time right);
	constexpr bool operator>=(Time left, Time right);
	constexpr Time operator-(Time right);
	constexpr Time operator+(Time left, Time right);
	constexpr Time operator-(Time left, Time right);
	constexpr Time operator*(Time left, Time right);
	constexpr Time operator/(Time left, Time right);
	constexpr Time operator+=(Time left, Time right);
	constexpr Time operator-=(Time left, Time right);
	constexpr Time operator*=(Time left, Time right);
	constexpr Time operator/=(Time left, Time right);

#include "Time.inl"
}
