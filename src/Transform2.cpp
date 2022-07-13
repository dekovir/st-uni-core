#include "unicore/Transform2.hpp"

namespace unicore
{
	Transform2& Transform2::operator*=(const Transform2& other)
	{
		const auto combined = *this * other;
		*this = combined;
		return *this;
	}
}