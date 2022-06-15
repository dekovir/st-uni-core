#pragma once
#include "unicore/Object.hpp"
#include <random>

namespace unicore
{
	class Random : public Object
	{
	public:
		virtual uint32_t next() = 0;

		virtual uint32_t range(uint32_t count);
		virtual bool boolean();

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		T range(T min, T max)
		{
			const auto value = range(max - min);
			return value + min;
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		inline T value_limit()
		{
			return static_cast<T>(Random::range(
				std::numeric_limits<T>::min(),
				std::numeric_limits<T>::max()));
		}
	};

	class DefaultRandom : public Random
	{
	public:
		DefaultRandom() = default;

		uint32_t next() override;

	protected:
		std::random_device _rd;
	};
}