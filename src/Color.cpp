#include "unicore/Color.hpp"

namespace unicore
{
	Color Color::create_random(Random& random)
	{
		return Color(
			random.value_limit<uint8_t>(),
			random.value_limit<uint8_t>(),
			random.value_limit<uint8_t>()
		);
	}

	bool operator==(const Color& a, const Color& b)
	{
		return
			a.r == b.r &&
			a.g == b.g &&
			a.b == b.b &&
			a.a == b.a;
	}
	
	bool operator!=(const Color& a, const Color& b)
	{
		return !(a == b);
	}
}
