#include "unicore/Color.hpp"
#include "unicore/Random.hpp"

namespace unicore
{
	Color Color::create_random()
	{
		return Color(
			Random::value_limit<uint8_t>(),
			Random::value_limit<uint8_t>(),
			Random::value_limit<uint8_t>()
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
