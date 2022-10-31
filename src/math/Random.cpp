#include "unicore/math/Random.hpp"

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

	Radians Random::radians()
	{
		return Radians(next_float_01() * Math::DoublePi);
	}

	Degrees Random::degrees()
	{
		return Degrees(next_float_01() * 360);
	}

	// DefaultRandom //////////////////////////////////////////////////////////////
	uint32_t DefaultRandom::next()
	{
		// TODO: Check range
		return _rd();
	}

	float DefaultRandom::next_float_01()
	{
		// TODO: Check range
		return static_cast <float> (next()) / static_cast <float> (std::random_device::max());
	}
}