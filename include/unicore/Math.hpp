#pragma once
#include "unicore/Defs.hpp"
#include <cmath>

namespace unicore
{
	class Radians;

	namespace Math
	{
		constexpr float PI = 3.14159265358979323846f;
		constexpr float DEG_TO_RAD = PI / 180;
		constexpr float RAD_TO_DEG = 180 / PI;

		static inline bool even(int value) { return (value % 2) == 0; }
		static inline bool odd(int value) { return (value % 2) == 1; }

		static inline int round(float value) { return (int)std::round(value); }
		static inline int ceil(float value) { return (int)std::ceil(value); }
		static inline int floor(float value) { return (int)std::floor(value); }

		template<typename T>
		static inline T min(T a, T b) { return a < b ? a : b; }

		template<typename T>
		static inline T max(T a, T b) { return a > b ? a : b; }

		static inline float sin(float value) { return std::sin(value); }
		static inline float cos(float value) { return std::cos(value); }
		static inline float pow(float value, int count = 2) { return std::pow(value, count); }

		static inline float to_rad(float radians) { return radians * DEG_TO_RAD; }
		static inline float to_deg(float radians) { return radians * RAD_TO_DEG; }

		static inline float sqrt(float value) { return std::sqrt(value); }

		template<typename T>
		static inline T abs(T value) { return std::abs(value); }

		template<typename T>
		static inline T sign(T value) { return value >= 0 ? +1 : -1; }
	}
}
