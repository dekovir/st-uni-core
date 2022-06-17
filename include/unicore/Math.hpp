#pragma once
#include "unicore/Defs.hpp"
#include <cmath>

namespace unicore
{
	namespace Math
	{
		constexpr float PI = 3.14159265358979323846f;
		constexpr float DEG_TO_RAD = PI / 180;
		constexpr float RAD_TO_DEG = 180 / PI;

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
		static inline float pow(float value, float count = 2) { return std::powf(value, count); }

		static inline float sqrt(float value) { return std::sqrt(value); }

		template<typename T>
		static inline T abs(T value) { return std::abs(value); }

		template<typename T>
		static constexpr T sign(T value) { return value >= 0 ? +1 : -1; }
	}
}