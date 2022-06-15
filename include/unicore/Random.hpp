#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	namespace Random
	{
		extern void set_seed(unsigned int seed);

		extern void set_random_seed();

		extern int value();

		extern int range(int min, int max);

		extern bool boolean();

		template<typename T>
		extern inline T value_limit()
		{
			return (T)Random::range(
				std::numeric_limits<T>::min(),
				std::numeric_limits<T>::max());
		}
	}
}