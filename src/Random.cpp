#include "unicore/Random.hpp"

namespace unicore
{
	uint32_t Random::range(uint32_t count)
	{
		return next() % count;
	}

	bool Random::boolean()
	{
		return range(0, 100) >= 50;
	}

	uint32_t DefaultRandom::next()
	{
		return _rd();
	}
}
