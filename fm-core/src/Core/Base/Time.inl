//#include "Core/Base/Time.h"

constexpr float Time::AsSeconds() const {

	return static_cast<float>(static_cast<double>(m_Microseconds) / 1000000.0);
}

constexpr Int32 Time::AsMilliseconds() const {

	return static_cast<Int32>(m_Microseconds / 1000);
}

constexpr Int64 Time::AsMicroseconds() const {

	return m_Microseconds;
}

inline constexpr Time Time::operator=(Time right) {

	m_Microseconds = right.AsMicroseconds();

	return *this;
}

constexpr Time Seconds(const float& amount) {

	return Time(static_cast<Int64>(amount * 1000000));
}

constexpr Time Milliseconds(const Int32& amount) {

	return Time(static_cast<Int64>(amount * 1000));
}

constexpr Time Microseconds(const Int64& amount) {

	return Time(amount);
}

constexpr bool operator==(Time left, Time right) {

	return left.AsMicroseconds() == right.AsMicroseconds();
}

constexpr bool operator!=(Time left, Time right) {

	return left.AsMicroseconds() != right.AsMicroseconds();
}

constexpr bool operator<(Time left, Time right) {

	return left.AsMicroseconds() < right.AsMicroseconds();
}

constexpr bool operator>(Time left, Time right) {

	return left.AsMicroseconds() > right.AsMicroseconds();
}

constexpr bool operator<=(Time left, Time right) {

	return left.AsMicroseconds() <= right.AsMicroseconds();
}

constexpr bool operator>=(Time left, Time right) {

	return left.AsMicroseconds() >= right.AsMicroseconds();
}

constexpr Time operator-(Time right) {

	return Time(-right.AsMicroseconds());
}

constexpr Time operator+(Time left, Time right) {

	return Time(left.AsMicroseconds() + right.AsMicroseconds());
}

constexpr Time operator-(Time left, Time right) {

	return Time(left.AsMicroseconds() - right.AsMicroseconds());
}

constexpr Time operator*(Time left, Time right) {

	return Time(left.AsMicroseconds() * right.AsMicroseconds());
}

constexpr Time operator/(Time left, Time right) {

	return Time(left.AsMicroseconds() / right.AsMicroseconds());
}

constexpr Time operator+=(Time left, Time right) {

	return left = left + right;
}

constexpr Time operator-=(Time left, Time right) {

	return left = left - right;
}

constexpr Time operator*=(Time left, Time right) {

	return left = left * right;
}

constexpr Time operator/=(Time left, Time right) {

	return left = left / right;
}

#ifdef FM_STATIC
inline constexpr Time Time::Zero;
#endif
