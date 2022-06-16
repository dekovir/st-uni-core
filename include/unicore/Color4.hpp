#pragma once
#include "unicore/Random.hpp"
#include "unicore/PixelFormat.hpp"

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
	class Color4
	{
	public:
		T r, g, b, a;

		static constexpr T MinValue = color_limits<T>::min();
		static constexpr T MaxValue = color_limits<T>::min();

		constexpr Color4() : r(MaxValue), g(MaxValue), b(MaxValue), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b)
			: r(_r), g(_g), b(_b), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b, T _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		template<
			typename DataType,
			typename ComponentType>
		static constexpr Color4 from_format(PixelFormat<DataType, ComponentType> format, DataType value)
		{
			const auto r = static_cast<uint8_t>(value >> format.r_shift) & format.component_mask;
			const auto g = static_cast<uint8_t>(value >> format.g_shift) & format.component_mask;
			const auto b = static_cast<uint8_t>(value >> format.b_shift) & format.component_mask;
			const auto a = static_cast<uint8_t>(value >> format.a_shift) & format.component_mask;
			return {
				convert_component_from_uint8(r),
				convert_component_from_uint8(g),
				convert_component_from_uint8(b),
				convert_component_from_uint8(a),
			};
		}

		template<
			typename DataType,
			typename ComponentType>
		constexpr DataType to_format(PixelFormat<DataType, ComponentType> format) const
		{
			return
				(convert_component_to_uint8(r) << format.r_shift) |
				(convert_component_to_uint8(g) << format.g_shift) |
				(convert_component_to_uint8(b) << format.b_shift) |
				(convert_component_to_uint8(a) << format.a_shift);
		}

		static constexpr Color4 from_rgb(const uint32_t rgb)
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

		[[nodiscard]] constexpr uint32_t to_rgb() const
		{
			return
				(convert_component_to_uint8(r) << 16) |
				(convert_component_to_uint8(g) << 8) |
				convert_component_to_uint8(b);
		}

		static constexpr Color4 from_argb(const uint32_t argb)
		{
			return from_format(pixel_format_argb, argb);
		}

		static constexpr Color4 from_rgba(const uint32_t rgba)
		{
			return from_format(pixel_format_rgba, rgba);
		}

		static Color4 create_random(Random& random)
		{
			return {
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
			};
		}

	protected:
		static constexpr T convert_component_from_uint8(uint8_t value)
		{
			if (std::is_same_v<T, uint8_t>) return value;

			if (std::is_floating_point_v<T>)
				return static_cast<T>((static_cast<T>(value) / 255.0f) * color_limits<T>::max());

			return static_cast<T>((static_cast<double>(value) / 255.0) * color_limits<T>::range()) + color_limits<T>::min();
		}

		[[nodiscard]] constexpr uint8_t convert_component_to_uint8(T value) const
		{
			if (std::is_same_v<T, uint8_t>) return value;

			if (std::is_floating_point_v<T>)
				return static_cast<uint8_t>((value / color_limits<T>::max()) * 0xFF);

			return static_cast<uint8_t>((static_cast<double>(value) / static_cast<double>(color_limits<T>::range())) * 0xFF);
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