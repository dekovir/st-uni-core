#include "unicore/Angle.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	const Radians Radians::Zero(0);
	const Degrees Degrees::Zero(0);

	Radians::Radians(const Degrees& value)
		: _value(Math::DEG_TO_RAD* value.get_value())
	{
	}

	Degrees::Degrees(const Radians& angle)
		: _value(Math::RAD_TO_DEG* angle.get_value())
	{
	}
}