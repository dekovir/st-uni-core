#include "unicore/math/Angle.hpp"
#include "unicore/system/StringBuilder.hpp"

namespace unicore
{
	UNICODE_STRING_BUILDER_FORMAT(const Radians&)
	{
		return builder << value.value() << "rad";
	}

	UNICODE_STRING_BUILDER_FORMAT(const Degrees&)
	{
		return builder << value.value() << "deg";
	}
}