#pragma once
#include "unicore/Defs.hpp"
#include <cmath>

namespace unicore
{
	namespace sfinae
	{
		template <class T, class = void>
		struct has_lerp_static_method : std::false_type {};

		template <class T>
		struct has_lerp_static_method<T, std::void_t<decltype(T::lerp)>> : std::true_type {};

		template <class T>
		inline constexpr bool has_lerp_static_method_t = has_lerp_static_method<T>::value;
	}

	namespace Math
	{
		constexpr Float Pi = 3.14159265358979323846f;
		constexpr Float DoublePi = Pi * 2;
		constexpr Float HalfPi = Pi / 2;

		constexpr Float DEG_TO_RAD = Pi / 180;
		constexpr Float RAD_TO_DEG = 180 / Pi;

		constexpr Float Epsilon = 1.192092896e-07F; // FLT_EPSILON

		namespace internal
		{
			template<typename T> struct floating_to_integral {};

			template<> struct floating_to_integral<float> { using Type = Int32; };
			template<> struct floating_to_integral<double> { using Type = Int64; };
		}

		// EVENT, ODD, ABS, SIGN, MOD ////////////////////////////////////////////////
		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		static constexpr bool even(T value) { return (value % 2) == 0; }

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		static constexpr bool odd(T value) { return (value % 2) == 1; }

		template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
		static constexpr T abs(T value)
		{
			return value < 0 ? -value : value;
		}

		template<typename T>
		static constexpr T sign(T value)
		{
			return value >= 0 ? static_cast<T>(+1) : static_cast<T>(-1);
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		static constexpr T mod(T value, T length) { return value % length; }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static constexpr T mod(T a, T b)
		{
			return a - b * static_cast<typename internal::floating_to_integral<T>::Type>(a / b);
		}

		// ROUND, CEIL, FLOOR ////////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T round(T value) { return std::round(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T ceil(T value) { return std::ceil(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T floor(T value) { return std::floor(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline auto round_to_int(T value)
		{
			return static_cast<typename internal::floating_to_integral<T>::Type>(round(value));
		}

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline auto ceil_to_int(T value)
		{
			return static_cast<typename internal::floating_to_integral<T>::Type>(ceil(value));
		}

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline auto floor_to_int(T value)
		{
			return static_cast<typename internal::floating_to_integral<T>::Type>(floor(value));
		}

		// MIN, MAX //////////////////////////////////////////////////////////////////
		template<typename T>
		static constexpr T min(T a, T b) { return a < b ? a : b; }

		template<typename T>
		static constexpr T max(T a, T b) { return a > b ? a : b; }

		// SIN, COS, ASIN, ACOS //////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T sin(T value) { return std::sin(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T cos(T value) { return std::cos(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T asin(T value) { return std::asin(value); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T acos(T value) { return std::acos(value); }

		// POW, SQRT /////////////////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T pow(T value, T count = 2) { return std::pow(value, count); }

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static inline T sqrt(T value) { return std::sqrt(value); }

		// EQUALS, COMPARE ///////////////////////////////////////////////////////////
		template<typename T>
		static constexpr bool equals(T a, T b)
		{
			return a == b;
		}

		template<>
		constexpr bool equals<Float>(Float a, Float b)
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

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
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

		// SNAP //////////////////////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		static T snap(T value, T snap)
		{
			if (equals(value, 0)) return value;
			return snap * round(value / snap);
		}

		// LERP //////////////////////////////////////////////////////////////////////
		template<typename T, std::enable_if_t<sfinae::has_lerp_static_method_t<T>>* = nullptr>
		static constexpr T lerp(const T& a, const T& b, float t)
		{
			return T::lerp(a, b, t);
		}

		template<typename T, std::enable_if_t<!sfinae::has_lerp_static_method_t<T>>* = nullptr>
		static constexpr T lerp(T a, T b, float t)
		{
			return static_cast<T>((b - a) * t + a);
		}

		template<typename T>
		static constexpr Float inverse_lerp(const T a, const T b, T t)
		{
			return static_cast<Float>(t - a) / static_cast<Float>(b - a);
		}

		// HASH //////////////////////////////////////////////////////////////////////
		template<typename T>
		static size_t hash(const T& value)
		{
			return std::hash<T>{}(value);
		}

		template<typename T, typename... Args>
		static size_t hash(T first, Args... args)
		{
			const size_t value = hash<T>(first) ^ (hash(args...) << 1);
			return value;
		}
	}

#define UNICORE_MATH_HASH(Type) \
	template<> inline size_t Math::hash<Type>(const Type& value)
}