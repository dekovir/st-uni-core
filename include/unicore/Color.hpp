#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	struct Color
	{
		uint8_t r, g, b, a;

		constexpr Color() : r(255), g(255), b(255), a(255) {}
		constexpr Color(uint8_t _r, uint8_t _g, uint8_t _b)
			: r(_r), g(_g), b(_b), a(255) {}
		constexpr Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		static constexpr Color from_rgb(uint32_t rgb)
		{
			return Color(rgb >> 16, (rgb >> 8) & 0xFF, rgb & 0xFF);
		}

		static constexpr Color from_rgba(uint32_t rgba)
		{
			return Color((rgba >> 16) & 0xFF, (rgba >> 8) & 0xFF, rgba & 0xFF, rgba >> 24);
		}

		static Color create_random();
	};

	static bool operator==(const Color& a, const Color& b);
	static bool operator!=(const Color& a, const Color& b);

	namespace Colors
	{
		constexpr Color Clear   = Color(0, 0, 0, 0);

		constexpr Color White   = Color::from_rgb(0xFFFFFF);
		constexpr Color Black   = Color::from_rgb(0x000000);

		constexpr Color Red     = Color::from_rgb(0xFF0000);
		constexpr Color Green   = Color::from_rgb(0x00FF00);
		constexpr Color Blue    = Color::from_rgb(0x0000FF);

		constexpr Color Magenta = Color::from_rgb(0xFF00FF);
		constexpr Color Yellow  = Color::from_rgb(0xFFFF00);
		constexpr Color Cyan    = Color::from_rgb(0x00FFFF);
	};
}