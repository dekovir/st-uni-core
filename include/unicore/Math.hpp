#pragma once
#include "unicore/Defs.hpp"
#include <cmath>

namespace unicore
{
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

		template<typename T>
		static constexpr T clamp(T value, T min_value, T max_value)
		{
			if (value > max_value) return max_value;
			if (value < min_value) return min_value;
			return value;
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

		template<typename T>
		static constexpr T lerp(T a, T b, float t)
		{
			return static_cast<T>((b - a) * t + a);
		}
	}

	struct AngleTypeRad
	{
		static inline float cos(float value) { return Math::cos(value); }
		static inline float sin(float value) { return Math::sin(value); }
	};

	struct AngleTypeDeg
	{
		static inline float to_rad(float value) { return Math::DEG_TO_RAD * value; }
		static inline float cos(float value) { return Math::cos(to_rad(value)); }
		static inline float sin(float value) { return Math::sin(to_rad(value)); }
	};

	template<typename TypeTag>
	class Angle
	{
	public:
		constexpr Angle() : _value(0) {}
		explicit constexpr Angle(float value) : _value(value) {}

		template<typename OtherTag>
		constexpr Angle(const Angle<OtherTag>& other)
			: _value(other.template cast<TypeTag>().value()) {}

		constexpr Angle(const Angle& other) = default;
		constexpr Angle(Angle&& other) noexcept = default;

		~Angle() = default;

		Angle& operator=(const Angle& other) = default;
		Angle& operator=(Angle&& other) noexcept = default;

		UC_NODISCARD constexpr float value() const { return _value; }

		UC_NODISCARD float cos() const { return TypeTag::cos(_value); }
		UC_NODISCARD float sin() const { return TypeTag::sin(_value); }

		void sin_cos(float& sin_value, float& cos_value) const
		{
			sin_value = TypeTag::sin(_value);
			cos_value = TypeTag::cos(_value);
		}

		Angle& operator+=(const Angle& other)
		{
			_value += other.value();
			return *this;
		}

		Angle& operator-=(const Angle& other)
		{
			_value -= other.value();
			return *this;
		}

		constexpr Angle operator-() const
		{
			return Angle(-_value);
		}

		template<typename OtherTag>
		constexpr Angle<OtherTag> cast() const;

	protected:
		float _value;
	};

	template <typename TypeTag>
	template <typename OtherTag>
	constexpr Angle<OtherTag> Angle<TypeTag>::cast() const
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented cast");
	}

	template <>
	template <>
	constexpr Angle<AngleTypeRad> Angle<AngleTypeDeg>::cast() const
	{
		return Angle<AngleTypeRad>(Math::DEG_TO_RAD * _value);
	}

	template <>
	template <>
	constexpr Angle<AngleTypeDeg> Angle<AngleTypeRad>::cast() const
	{
		return Angle<AngleTypeDeg>(Math::RAD_TO_DEG * _value);
	}

	template<typename TypeTag>
	static constexpr bool operator==(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() == b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator!=(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() != b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator>(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() > b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator<(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() < b.value();
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator+(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return Angle<TypeTag>(a.value() + b.value());
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator-(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return Angle<TypeTag>(a.value() - b.value());
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator*(const Angle<TypeTag>& a, float value)
	{
		return Angle<TypeTag>(a.value() * value);
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator/(const Angle<TypeTag>& a, float value)
	{
		return Angle<TypeTag>(a.value() / value);
	}

	using Degrees = Angle<AngleTypeDeg>;
	using Radians = Angle<AngleTypeRad>;

	template<typename AngleTag>
	struct AngleConst
	{
		static constexpr auto Zero = Angle<AngleTag>(0);
		static constexpr auto Pi = Angle<AngleTag>(Angle<AngleTypeRad>(Math::Pi));
		static constexpr auto PiHalf = Angle<AngleTag>(Angle<AngleTypeRad>(Math::Pi / 2));
	};

	using DegreesConst = AngleConst<AngleTypeDeg>;
	using RadiansConst = AngleConst<AngleTypeRad>;

	static constexpr Radians operator"" _rad(unsigned long long value)
	{
		return Radians(static_cast<float>(value));
	}

	static constexpr Radians operator"" _rad(long double value)
	{
		return Radians(static_cast<float>(value));
	}

	static constexpr Degrees operator"" _deg(unsigned long long value)
	{
		return Degrees(static_cast<float>(value));
	}

	static constexpr Degrees operator"" _deg(long double value)
	{
		return Degrees(static_cast<float>(value));
	}
}