#pragma once
#include "unicore/Object.hpp"
#include <random>

namespace unicore
{
	class Random : public Object
	{
		UC_OBJECT(Random, Object)
	public:
		virtual uint32_t next() = 0;
		virtual float next_float() = 0;

		virtual uint32_t range(uint32_t count);
		virtual bool boolean();

		template<typename T, std::enable_if_t<std::is_signed_v<T>>* = nullptr>
		T sign()
		{
			return boolean() ? static_cast<T>(-1) : static_cast<T>(+1);
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		T range(T min, T max)
		{
			const auto value = range(max - min);
			return value + min;
		}

		float range(float min, float max)
		{
			const auto delta = max - min;
			return next_float() * delta + min;
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		T value_limit()
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
		float next_float() override;

	protected:
		std::random_device _rd;
	};
}