#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Degrees;

	class Radians
	{
	public:
		constexpr Radians() : _value(0) {}
		explicit constexpr Radians(float value) : _value(value) {}
		explicit Radians(const Degrees& value);

		UC_NODISCARD constexpr float get_value() const { return _value; }

		constexpr bool operator < (const Radians& angle) const { return _value < angle._value; }
		constexpr bool operator > (const Radians& angle) const { return _value > angle._value; }

		static const Radians Zero;

	protected:
		float _value;
	};

	class Degrees
	{
	public:
		constexpr Degrees() : _value(0) {}
		explicit constexpr Degrees(float value) : _value(value) {}
		explicit Degrees(const Radians& angle);

		UC_NODISCARD constexpr float get_value() const { return _value; }

		constexpr bool operator < (const Degrees& angle) const { return _value < angle._value; }
		constexpr bool operator > (const Degrees& angle) const { return _value > angle._value; }

		static const Degrees Zero;

	protected:
		float _value;
	};

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