#pragma once

#include <sstream>

#include "Core/Base/Base.h"
#include "Core/Base/Vector4.h"
#include "Core/Base/Vector3.h"

namespace fm {

	class FM_API Color {

	public:

		constexpr Color(byte r, byte g, byte b, byte a)
			: r(r), g(g), b(b), a(a)

		{}

		constexpr Color(byte r, byte g, byte b)
			: r(r), g(g), b(b), a(255)

		{}

		constexpr Color(Uint32 color)
			:
			r(static_cast<std::uint8_t>((color & 0xff000000) >> 24)),
			g(static_cast<std::uint8_t>((color & 0x00ff0000) >> 16)),
			b(static_cast<std::uint8_t>((color & 0x0000ff00) >> 8)),
			a(static_cast<std::uint8_t>(color & 0x000000ff))

		{}

		constexpr Color(const Vector4u8& color)
			: r(color.r), g(color.g), b(color.b), a(color.a)

		{}

		constexpr Color(const Vector4f& color)
			: r(byte(color.r * 255.0f)), g(byte(color.g * 255.0f)), b(byte(color.b * 255.0f)), a(byte(color.a * 255.0f))

		{}

		constexpr Color(const Vector3f& color)
			: r(byte(color.r * 255.0f)), g(byte(color.g * 255.0f)), b(byte(color.b * 255.0f)), a(255)

		{}

		constexpr Color()
			: r(0), g(0), b(0), a(255)

		{}

		constexpr inline Uint32 ToInteger() const {

			return static_cast<std::uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
		}

		constexpr inline const Vector4f ToVector4f() const {

			return { (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f };
		}

		inline const std::string ToString() const {

			std::stringstream ss;
			ss << "(" << (int)r << ", " << (int)g << ", " << (int)b << ", " << (int)a << ")";
			return ss.str();
		}

		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Transparent;

		byte r;
		byte g;
		byte b;
		byte a;
	};

	[[nodiscard]] constexpr bool operator==(const Color& left, const Color& right);

	[[nodiscard]] constexpr bool operator!=(const Color& left, const Color& right);

	[[nodiscard]] constexpr Color operator+(const Color& left, const Color& right);

	[[nodiscard]] constexpr Color operator-(const Color& left, const Color& right);

	[[nodiscard]] constexpr Color operator*(const Color& left, const Color& right);

	constexpr Color& operator+=(Color& left, const Color& right);

	constexpr Color& operator-=(Color& left, const Color& right);

	constexpr Color& operator*=(Color& left, const Color& right);

#include <Core/Base/Color.inl>
}
