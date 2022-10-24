#pragma once
#include "unicore/math/Math.hpp"
#include "unicore/math/Hash.hpp"
#include "unicore/renderer/PixelFormat.hpp"

namespace unicore
{
	template<typename T>
	class Color4
	{
	public:
		T r, g, b, a;

		using Limits = color_limits<T>;

		static constexpr T MinValue = 0;
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

		// TODO: Test convert
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
				color_limits_convert::component<UInt8, T>(r),
				color_limits_convert::component<UInt8, T>(g),
				color_limits_convert::component<UInt8, T>(b),
				color_limits_convert::component<UInt8, T>(a)
			};
		}

		// TODO: Test convert
		template<
			typename DataType,
			typename ComponentType>
		UC_NODISCARD constexpr DataType to_format(PixelFormat<DataType, ComponentType> format) const
		{
			return
				(color_limits_convert::component<T, ComponentType>(r) << format.r_shift) |
				(color_limits_convert::component<T, ComponentType>(g) << format.g_shift) |
				(color_limits_convert::component<T, ComponentType>(b) << format.b_shift) |
				(color_limits_convert::component<T, ComponentType>(a) << format.a_shift);
		}

		UC_NODISCARD constexpr Color4 invert() const
		{
			return {
				static_cast<T>(MaxValue - r),
				static_cast<T>(MaxValue - g),
				static_cast<T>(MaxValue - b),
				a
			};
		}

		template<typename U>
		UC_NODISCARD constexpr Color4<U> cast() const
		{
			return {
				color_limits_convert::component<U>(r),
				color_limits_convert::component<U>(g),
				color_limits_convert::component<U>(b),
				color_limits_convert::component<U>(a)
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
				color_limits_convert::component<UInt8, T>(r),
				color_limits_convert::component<UInt8, T>(g),
				color_limits_convert::component<UInt8, T>(b),
				MaxValue
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

	using Color4b = Color4<Byte>;
	using Color4f = Color4<Float>;

	namespace details
	{
		template<typename T>
		struct ColorConst4
		{
			static constexpr auto MaxValue = color_limits<T>::max();

			static constexpr auto Clear = Color4<T>(0, 0, 0, 0);

			static constexpr auto Black = Color4<T>(0, 0, 0);
			static constexpr auto White = Color4<T>(MaxValue, MaxValue, MaxValue);

			static constexpr auto Red = Color4<T>(MaxValue, 0, 0);
			static constexpr auto Green = Color4<T>(0, MaxValue, 0);
			static constexpr auto Blue = Color4<T>(0, 0, MaxValue);

			static constexpr auto Magenta = Color4<T>(MaxValue, 0, MaxValue);
			static constexpr auto Yellow = Color4<T>(MaxValue, MaxValue, 0);
			static constexpr auto Cyan = Color4<T>(0, MaxValue, MaxValue);

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

	UNICORE_MAKE_HASH(Color4b)
	{
		return value.to_format(pixel_format_abgr);
	}

	UNICORE_MAKE_HASH(Color4f)
	{
		return make(value.r, value.g, value.b, value.a);
	}
}