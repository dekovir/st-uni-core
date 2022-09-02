#pragma once
#include "unicore/Defs.hpp"
#include <cmath>

namespace unicore
{
	namespace sfinae
	{
		template <class T, class = void>
		struct has_lerp_static_method : public std::false_type {};

		template <class T>
		struct has_lerp_static_method<T, std::void_t<decltype(T::lerp)>> : public std::true_type {};

		template <class T>
		inline constexpr bool has_lerp_static_method_t = has_lerp_static_method<T>::value;
	}

	namespace Math
	{
		constexpr float Pi = 3.14159265358979323846f;
		constexpr float DoublePi = Pi * 2;
		constexpr float HalfPi = Pi / 2;

		constexpr float DEG_TO_RAD = Pi / 180;
		constexpr float RAD_TO_DEG = 180 / Pi;

		constexpr float Epsilon = 1.192092896e-07F; // FLT_EPSILON

		static constexpr bool even(int value) { return (value % 2) == 0; }
		static constexpr bool odd(int value) { return (value % 2) == 1; }

		static inline int round(float value) { return static_cast<int>(std::round(value)); }
		static inline int ceil(float value) { return static_cast<int>(std::ceil(value)); }
		static inline int floor(float value) { return static_cast<int>(std::floor(value)); }

		template<typename T>
		static constexpr T min(T a, T b) { return a < b ? a : b; }

		template<typename T>
		static constexpr T max(T a, T b) { return a > b ? a : b; }

		static inline float sin(float value) { return std::sin(value); }
		static inline float cos(float value) { return std::cos(value); }

		static inline float asin(float value) { return std::asin(value); }
		static inline float acos(float value) { return std::acos(value); }

		static inline float pow(float value, float count = 2) { return std::pow(value, count); }

		static inline float sqrt(float value) { return std::sqrt(value); }

		template<typename T,
			std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
		static constexpr T abs(T value)
		{
			return value < 0 ? -value : value;
		}

		template<typename T>
		static constexpr T sign(T value) { return value >= 0 ? +1 : -1; }

		template<typename T>
		static constexpr bool equals(T a, T b)
		{
			return a == b;
		}

		template<>
		constexpr bool equals<float>(float a, float b)
		{
			return abs(a - b) < Epsilon;
		}

		template<typename T>
		static constexpr int compare(T a, T b)
		{
			if (a > b) return +1;
			if (a < b) return -1;
			return 0;
		}

		// CLAMP /////////////////////////////////////////////////////////////////////
		template<typename T>
		static constexpr T clamp(T value, T min_value, T max_value)
		{
			if (value > max_value) return max_value;
			if (value < min_value) return min_value;
			return value;
		}

		template<typename T,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static constexpr T clamp_01(T value)
		{
			return clamp(value, static_cast<T>(0), static_cast<T>(1));
		}

		template<typename T>
		static constexpr T clamp_max(T value, T max_value)
		{
			if (value > max_value) return max_value;
			return value;
		}

		template<typename T>
		static constexpr T clamp_min(T value, T min_value)
		{
			if (value < min_value) return min_value;
			return value;
		}

		// LERP //////////////////////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<sfinae::has_lerp_static_method_t<T>>* = nullptr>
		static constexpr T lerp(T a, T b, float t)
		{
			return T::lerp(a, b, t);
		}

		template<typename T, std::enable_if_t<!sfinae::has_lerp_static_method_t<T>>* = nullptr>
		static constexpr T lerp(T a, T b, float t)
		{
			return static_cast<T>((b - a) * t + a);
		}
	}
}