#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Degrees;

	class Radians
	{
	public:
		Radians() = default;
		explicit Radians(float value) : _value(value) {}
		Radians(const Degrees& value);

		UC_NODISCARD float get_value() const { return _value; }

		bool operator < (const Radians& angle) const { return _value < angle._value; }
		bool operator > (const Radians& angle) const { return _value > angle._value; }

		static const Radians Zero;

	protected:
		float _value;
	};

	class Degrees
	{
	public:
		Degrees() = default;
		explicit Degrees(float value) : _value(value) {}
		Degrees(const Radians& angle);

		UC_NODISCARD float get_value() const { return _value; }

		bool operator < (const Degrees& angle) const { return _value < angle._value; }
		bool operator > (const Degrees& angle) const { return _value > angle._value; }

		static const Degrees Zero;

	protected:
		float _value;
	};
}