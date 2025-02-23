
constexpr bool operator==(const Color& left, const Color& right) {

	return (left.r == right.r) && (left.g == right.g) && (left.b == right.b) && (left.a == right.a);
}

constexpr bool operator!=(const Color& left, const Color& right) {

	return !(left == right);
}

constexpr Color operator+(const Color& left, const Color& right) {

	const auto clampedAdd = [](byte lhs, byte rhs)->byte {

		const int intResult = int{ lhs } + int{ rhs };
		return static_cast<byte>(intResult < 255 ? intResult : 255);
	};

	return { clampedAdd(left.r, right.r),
			clampedAdd(left.g, right.g),
			clampedAdd(left.b, right.b),
			clampedAdd(left.a, right.a) };
}

constexpr Color operator-(const Color& left, const Color& right) {

	const auto clampedSub = [](byte lhs, byte rhs) -> byte {

		const int intResult = int{ lhs } - int{ rhs };
		return static_cast<byte>(intResult > 0 ? intResult : 0);
	};

	return { clampedSub(left.r, right.r),
			clampedSub(left.g, right.g),
			clampedSub(left.b, right.b),
			clampedSub(left.a, right.a) };
}

constexpr Color operator*(const Color& left, const Color& right) {

	const auto scaledMul = [](byte lhs, byte rhs) -> byte {

			const auto uint16Result = static_cast<Uint16>(Uint16{ lhs } *Uint16{ rhs });
			return static_cast<byte>(uint16Result / 255u);
		};

	return { scaledMul(left.r, right.r), scaledMul(left.g, right.g), scaledMul(left.b, right.b), scaledMul(left.a, right.a) };
}

constexpr Color& operator+=(Color& left, const Color& right) {

	return left = left + right;
}

constexpr Color& operator-=(Color& left, const Color& right) {

	return left = left - right;
}

constexpr Color& operator*=(Color& left, const Color& right) {

	return left = left * right;
}

#ifdef FM_STATIC
inline constexpr Color Color::Black(0, 0, 0);
inline constexpr Color Color::White(255, 255, 255);
inline constexpr Color Color::Red(255, 0, 0);
inline constexpr Color Color::Green(0, 255, 0);
inline constexpr Color Color::Blue(0, 0, 255);
inline constexpr Color Color::Yellow(255, 255, 0);
inline constexpr Color Color::Magenta(255, 0, 255);
inline constexpr Color Color::Cyan(0, 255, 255);
inline constexpr Color Color::Transparent(0, 0, 0, 0);
#endif
