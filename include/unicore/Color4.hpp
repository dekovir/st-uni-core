#pragma once
#include "unicore/Math.hpp"
#include "unicore/Random.hpp"
#include "unicore/PixelFormat.hpp"

namespace unicore
{
	template<typename T>
	class Color4
	{
	public:
		T r, g, b, a;

		using Limits = color_limits<T>;

		static constexpr T MinValue = Limits::min();
		static constexpr T MaxValue = Limits::max();

		constexpr Color4() : r(MaxValue), g(MaxValue), b(MaxValue), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b)
			: r(_r), g(_g), b(_b), a(MaxValue) {}
		constexpr Color4(T _r, T _g, T _b, T _a)
			: r(_r), g(_g), b(_b), a(_a) {}

		// Copy constructor
		constexpr Color4(const Color4& other) = default;

		// Move constructor
		constexpr Color4(Color4&& other) noexcept
			: r(other.r), g(other.g), b(other.b), a(other.a) {}

		~Color4() = default;

		// Copy assignment operator
		Color4& operator=(const Color4& other) noexcept = default;

		// Move assignment operator
		Color4& operator=(Color4&& other) noexcept
		{
			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;
			return  *this;
		}

		template<
			typename DataType,
			typename ComponentType>
		static constexpr Color4 from_format(PixelFormat<DataType, ComponentType> format, DataType value)
		{
			const auto r = static_cast<uint8_t>((value >> format.r_shift) & format.component_mask);
			const auto g = static_cast<uint8_t>((value >> format.g_shift) & format.component_mask);
			const auto b = static_cast<uint8_t>((value >> format.b_shift) & format.component_mask);
			const auto a = static_cast<uint8_t>((value >> format.a_shift) & format.component_mask);
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

		UC_NODISCARD constexpr uint32_t to_rgb() const
		{
			return
				(convert_component_to_uint8(r) << 16) |
				(convert_component_to_uint8(g) << 8) |
				convert_component_to_uint8(b);
		}

		UC_NODISCARD constexpr Color4 invert() const
		{
			return {
				static_cast<T>(Limits::max() - r),
				static_cast<T>(Limits::max() - g),
				static_cast<T>(Limits::max() - b),
				a
			};
		}

		static constexpr T clamp_value(T value)
		{
			return Math::clamp(value, MinValue, MaxValue);
		}

		static constexpr Color4 lerp(const Color4& a, const Color4& b, float t)
		{
			return {
				Math::lerp(a.r, b.r, t),
				Math::lerp(a.g, b.g, t),
				Math::lerp(a.b, b.b, t),
				Math::lerp(a.a, b.a, t),
			};
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
				Limits::max()
			};
		}

		static constexpr Color4 from_argb(const uint32_t argb)
		{
			return from_format(pixel_format_argb, argb);
		}

		static constexpr Color4 from_rgba(const uint32_t rgba)
		{
			return from_format(pixel_format_rgba, rgba);
		}

		static Color4 create_random(Random& random, bool random_alpha = false)
		{
			return {
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
				!random_alpha ? MaxValue : random.range(MinValue, MaxValue)
			};
		}

	protected:
		// TODO: Convert to template
		static constexpr T convert_component_from_uint8(uint8_t value)
		{
			if constexpr (std::is_same_v<T, uint8_t>) return value;

			if constexpr (std::is_floating_point_v<T>)
				return static_cast<T>((static_cast<T>(value) / 255.0f) * Limits::max());

			return static_cast<T>((static_cast<double>(value) / 255.0) * Limits::range()) + Limits::min();
		}

		// TODO: Convert to template
		UC_NODISCARD constexpr uint8_t convert_component_to_uint8(T value) const
		{
			if constexpr (std::is_same_v<T, uint8_t>) return value;

			if constexpr (std::is_floating_point_v<T>)
				return static_cast<uint8_t>((value / Limits::max()) * 0xFF);

			return static_cast<uint8_t>((static_cast<double>(value) / static_cast<double>(Limits::range())) * 0xFF);
		}
	};

	template<typename T>
	static constexpr bool operator==(const Color4<T>& a, const Color4<T>& b)
	{
		return
			Math::equals(a.r, b.r) &&
			Math::equals(a.g, b.g) &&
			Math::equals(a.b, b.b) &&
			Math::equals(a.a, b.a);
	}

	template<typename T>
	static constexpr bool operator!=(const Color4<T>& a, const Color4<T>& b)
	{
		return !(a == b);
	}

	template<typename T, std::enable_if<std::is_floating_point_v<T>>* = nullptr>
	static constexpr Color4<T> operator*(const Color4<T>& a, const Color4<T>& b)
	{
		return {
			a.r * b.r,
			a.g * b.g,
			a.b * b.b,
			a.a * b.a,
		};
	}

	template<typename T>
	static constexpr Color4<T> operator*(const Color4<T>& color,
		typename Color4<T>::Limits::ValueType value)
	{
		return {
			Color4<T>::clamp_value(color.r * value),
			Color4<T>::clamp_value(color.g * value),
			Color4<T>::clamp_value(color.b * value),
			Color4<T>::clamp_value(color.a * value),
		};
	}

	template<typename T>
	static constexpr Color4<T> operator/(const Color4<T>& color,
		typename Color4<T>::Limits::ValueType value)
	{
		return {
			Color4<T>::clamp_value(color.r / value),
			Color4<T>::clamp_value(color.g / value),
			Color4<T>::clamp_value(color.b / value),
			Color4<T>::clamp_value(color.a / value),
		};
	}

	using Color4b = Color4<uint8_t>;
	using Color4f = Color4<float>;

	namespace details
	{
		template<typename T>
		struct ColorConst4
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

			ColorConst4() = delete;
		};

		template<typename T>
		using ColorPalette4 = List<Color4<T>>;
	}

	using ColorConst4b = details::ColorConst4<uint8_t>;
	using ColorConst4f = details::ColorConst4<float>;

	using ColorPalette4b = details::ColorPalette4<uint8_t>;
	using ColorPalette4f = details::ColorPalette4<float>;

	//static constexpr Color4b operator"" _argb(unsigned long long value)
	//{
	//	return Color4b::from_argb(static_cast<uint32_t>(value));
	//}

	//static constexpr Color4b operator"" _rgba(unsigned long long value)
	//{
	//	return Color4b::from_rgba(static_cast<uint32_t>(value));
	//}

	UNICORE_MATH_HASH(Color4b)
	{
		return value.to_format(pixel_format_abgr);
	}

	UNICORE_MATH_HASH(Color4f)
	{
		return hash(value.r, value.g, value.b, value.a);
	}
}