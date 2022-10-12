#pragma once
#include "unicore/math/Math.hpp"
#include "unicore/math/Hash.hpp"
#include "unicore/math/Random.hpp"
#include "unicore/PixelFormat.hpp"

namespace unicore
{
	template<typename T>
	class Color3
	{
	public:
		T r, g, b;

		using Limits = color_limits<T>;

		static constexpr T MinValue = Limits::min();
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

		template<
			typename DataType,
			typename ComponentType>
		static constexpr Color3 from_format(PixelFormat<DataType, ComponentType> format, DataType value)
		{
			const auto r = static_cast<uint8_t>((value >> format.r_shift) & format.component_mask);
			const auto g = static_cast<uint8_t>((value >> format.g_shift) & format.component_mask);
			const auto b = static_cast<uint8_t>((value >> format.b_shift) & format.component_mask);
			return {
				convert_component_from_uint8(r),
				convert_component_from_uint8(g),
				convert_component_from_uint8(b),
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
				(convert_component_to_uint8(b) << format.b_shift);
		}

		UC_NODISCARD constexpr uint32_t to_rgb() const
		{
			return
				(convert_component_to_uint8(r) << 16) |
				(convert_component_to_uint8(g) << 8) |
				convert_component_to_uint8(b);
		}

		UC_NODISCARD constexpr Color3 invert() const
		{
			return {
				static_cast<T>(Limits::max() - r),
				static_cast<T>(Limits::max() - g),
				static_cast<T>(Limits::max() - b)
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

		static constexpr Color3 from_rgb(const uint32_t rgb)
		{
			const uint8_t r = (rgb >> 16) & 0xFF;
			const uint8_t g = (rgb >> 8) & 0xFF;
			const uint8_t b = rgb & 0xFF;
			return {
				convert_component_from_uint8(r),
				convert_component_from_uint8(g),
				convert_component_from_uint8(b),
			};
		}

		static Color3 create_random(Random& random)
		{
			return {
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
				random.range(MinValue, MaxValue),
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

	namespace details
	{
		template<typename T>
		struct ColorConst3
		{
			static constexpr auto Clear = Color3<T>::from_argb(0);

			static constexpr auto Black = Color3<T>::from_rgb(0x000000);
			static constexpr auto White = Color3<T>::from_rgb(0xFFFFFF);

			static constexpr auto Red = Color3<T>::from_rgb(0xFF0000);
			static constexpr auto Green = Color3<T>::from_rgb(0x00FF00);
			static constexpr auto Blue = Color3<T>::from_rgb(0x0000FF);

			static constexpr auto Magenta = Color3<T>::from_rgb(0xFF00FF);
			static constexpr auto Yellow = Color3<T>::from_rgb(0xFFFF00);
			static constexpr auto Cyan = Color3<T>::from_rgb(0x00FFFF);

			ColorConst3() = delete;
		};

		template<typename T>
		using ColorPalette3 = List<Color3<T>>;
	}

	using ColorConst3b = details::ColorConst3<uint8_t>;
	using ColorConst3f = details::ColorConst3<float>;

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
}