#pragma once
#include "unicore/math/Math.hpp"
#include "unicore/math/Hash.hpp"
#include "unicore/renderer/PixelFormat.hpp"
#include "unicore/renderer/ColorTable.hpp"

namespace unicore
{
	template<typename T>
	class Color3
	{
	public:
		T r, g, b;

		using Limits = color_limits<T>;

		static constexpr T MinValue = 0;
		static constexpr T MaxValue = Limits::max();

		constexpr Color3() : r(MaxValue), g(MaxValue), b(MaxValue) {}
		constexpr Color3(T _r, T _g, T _b) : r(_r), g(_g), b(_b) {}

		// Copy constructor
		constexpr Color3(const Color3& other) = default;

		// Move constructor
		constexpr Color3(Color3&& other) noexcept
			: r(other.r), g(other.g), b(other.b) {}

		~Color3() = default;

		// Copy assignment operator
		Color3& operator=(const Color3& other) noexcept = default;

		// Move assignment operator
		Color3& operator=(Color3&& other) noexcept
		{
			r = other.r;
			g = other.g;
			b = other.b;
			return  *this;
		}

		// TODO: Test convert
		template<
			typename DataType,
			typename ComponentType>
		static constexpr Color3 from_format(PixelFormat<DataType, ComponentType> format, DataType value)
		{
			const auto r = static_cast<uint8_t>((value >> format.r_shift) & format.component_mask);
			const auto g = static_cast<uint8_t>((value >> format.g_shift) & format.component_mask);
			const auto b = static_cast<uint8_t>((value >> format.b_shift) & format.component_mask);
			return {
				color_limits_convert::component<UInt8, T>(r),
				color_limits_convert::component<UInt8, T>(g),
				color_limits_convert::component<UInt8, T>(b)
			};
		}

		// TODO: Test convert
		template<
			typename DataType,
			typename ComponentType>
		constexpr DataType to_format(PixelFormat<DataType, ComponentType> format) const
		{
			return
				(color_limits_convert::component<T, ComponentType>(r) << format.r_shift) |
				(color_limits_convert::component<T, ComponentType>(g) << format.g_shift) |
				(color_limits_convert::component<T, ComponentType>(b) << format.b_shift);
		}

		UC_NODISCARD constexpr Color3 invert() const
		{
			return {
				static_cast<T>(MaxValue - r),
				static_cast<T>(MaxValue - g),
				static_cast<T>(MaxValue - b)
			};
		}

		template<typename U>
		UC_NODISCARD constexpr Color3<U> cast() const
		{
			return {
				color_limits_convert::component<T, U>(r),
				color_limits_convert::component<T, U>(g),
				color_limits_convert::component<T, U>(b)
			};
		}

		static constexpr T clamp_value(T value)
		{
			return Math::clamp(value, MinValue, MaxValue);
		}

		static constexpr Color3 lerp(const Color3& a, const Color3& b, float t)
		{
			return {
				Math::lerp(a.r, b.r, t),
				Math::lerp(a.g, b.g, t),
				Math::lerp(a.b, b.b, t),
			};
		}

		static constexpr Color3 from_one(T value)
		{
			return { value, value, value };
		}

		static constexpr Color3 from_rgb(const uint32_t rgb)
		{
			const UInt8 r = (rgb >> 16) & 0xFF;
			const UInt8 g = (rgb >> 8) & 0xFF;
			const UInt8 b = rgb & 0xFF;
			return {
				color_limits_convert::component<UInt8, T>(r),
				color_limits_convert::component<UInt8, T>(g),
				color_limits_convert::component<UInt8, T>(b)
			};
		}
	};

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr bool operator==(const Color3<T>& a, const Color3<T>& b)
	{
		return
			Math::equals(a.r, b.r) &&
			Math::equals(a.g, b.g) &&
			Math::equals(a.b, b.b);
	}

	template<typename T>
	static constexpr bool operator!=(const Color3<T>& a, const Color3<T>& b)
	{
		return !(a == b);
	}

	template<typename T, std::enable_if<std::is_floating_point_v<T>>* = nullptr>
	static constexpr Color3<T> operator*(const Color3<T>& a, const Color3<T>& b)
	{
		return {
			a.r * b.r,
			a.g * b.g,
			a.b * b.b,
		};
	}

	template<typename T>
	static constexpr Color3<T> operator*(const Color3<T>& color,
		typename Color3<T>::Limits::ValueType value)
	{
		return {
			Color3<T>::clamp_value(color.r * value),
			Color3<T>::clamp_value(color.g * value),
			Color3<T>::clamp_value(color.b * value),
		};
	}

	template<typename T>
	static constexpr Color3<T> operator/(const Color3<T>& color,
		typename Color3<T>::Limits::ValueType value)
	{
		return {
			Color3<T>::clamp_value(color.r / value),
			Color3<T>::clamp_value(color.g / value),
			Color3<T>::clamp_value(color.b / value),
		};
	}

	using Color3b = Color3<uint8_t>;
	using Color3f = Color3<float>;

	static_assert(sizeof(Color3f) == sizeof(float) * 3);
	static_assert(std::is_polymorphic_v<Color3f> == false);

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		using ColorPalette3 = List<Color3<T>>;
	}

	using ColorConst3b = ColorTable<Color3b>;
	using ColorConst3f = ColorTable<Color3f>;

	using ColorPalette3b = details::ColorPalette3<uint8_t>;
	using ColorPalette3f = details::ColorPalette3<float>;

	UNICORE_MAKE_HASH(Color3b)
	{
		return make(value.r, value.g, value.b);
	}

	UNICORE_MAKE_HASH(Color3f)
	{
		return make(value.r, value.g, value.b);
	}

	template<typename T>
	extern UNICODE_STRING_BUILDER_FORMAT(const Color3<T>&)
	{
		return builder << value.r << ":" << value.g << ":" << value.b;
	}
}