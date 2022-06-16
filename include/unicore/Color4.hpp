#pragma once
#include "unicore/Random.hpp"

namespace unicore
{
	template<typename T>
	struct color_limits
	{
		static constexpr T min() noexcept { return std::numeric_limits<T>::min(); }
		static constexpr T max() noexcept { return std::numeric_limits<T>::max(); }
		static constexpr T range() noexcept { return std::numeric_limits<T>::max() - std::numeric_limits<T>::min(); }
	};

	template<>
	struct color_limits<float>
	{
		static constexpr float min() noexcept { return 0.0f; }
		static constexpr float max() noexcept { return 1.0f; }
		static constexpr float range() noexcept { return 1.0f; }
	};

	template<typename T>
	struct Color4
	{
		T r, g, b, a;

		static constexpr T MinValue = color_limits<T>::min();
		static constexpr T MaxValue = color_limits<T>::min();

		constexpr Color4() : r(MaxValue), g(MaxValue), b(MaxValue), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b)
			: r(_r), g(_g), b(_b), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b, T _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		static constexpr T convert_component_from_uint8(uint8_t value)
		{
			if (std::is_same_v<T, uint8_t>) return value;

			if (std::is_floating_point_v<T>)
				return static_cast<T>((static_cast<T>(value) / 255.f) * color_limits<T>::max());

			return static_cast<T>((static_cast<double>(value) / 255.0f) * color_limits<T>::range()) + color_limits<T>::min();
		}

		static constexpr Color4 from_rgb(uint32_t rgb)
		{
			const uint8_t r = (rgb >> 16) & 0xFF;
			const uint8_t g = (rgb >> 8) & 0xFF;
			const uint8_t b = rgb & 0xFF;
			return {
				convert_component_from_uint8(r),
				convert_component_from_uint8(g),
				convert_component_from_uint8(b),
				color_limits<T>::max()
			};
		}

		template<typename U = T, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
		static constexpr Color4 from_argb(uint32_t rgba)
		{
			const uint8_t a = (rgba >> 24) & 0xFF;
			const uint8_t r = (rgba >> 16) & 0xFF;
			const uint8_t g = (rgba >> 8) & 0xFF;
			const uint8_t b = rgba & 0xFF;
			return {
				convert_component_from_uint8(r),
				convert_component_from_uint8(g),
				convert_component_from_uint8(b),
				convert_component_from_uint8(a),
			};
		}

		static Color4 create_random(Random& random)
		{
			return {
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
			};
		}
	};

	template<typename T>
	static bool operator==(const Color4<T>& a, const Color4<T>& b)
	{
		return
			a.r == b.r &&
			a.g == b.g &&
			a.b == b.b &&
			a.a == b.a;
	}

	template<typename T>
	static bool operator!=(const Color4<T>& a, const Color4<T>& b)
	{
		return !(a == b);
	}

	using Color4b = Color4<uint8_t>;
	using Color4f = Color4<float>;

	template<typename T>
	struct Color4Table
	{
		static constexpr auto Clear = Color4<T>::from_argb(0);

		static constexpr auto Black = Color4<T>::from_rgb(0x000000);
		static constexpr auto White = Color4<T>::from_rgb(0xFFFFFF);

		static constexpr auto Red = Color4<T>::from_rgb(0xFF0000);
		static constexpr auto Green = Color4<T>::from_rgb(0x00FF00);
		static constexpr auto Blue = Color4<T>::from_rgb(0x0000FF);

		static constexpr auto Magenta = Color4<T>::from_rgb(0xFF00FF);
		static constexpr auto Yellow = Color4<T>::from_rgb(0xFFFF00);
		static constexpr auto Cyan = Color4<T>::from_rgb(0x00FFFF);
	};

	using Colors4b = Color4Table<uint8_t>;
	using Colors4f = Color4Table<float>;
}