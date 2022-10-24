#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/math/Range.hpp"
#include "unicore/renderer/Color3.hpp"
#include "unicore/renderer/Color4.hpp"
#include <random>

namespace unicore
{
	class Random : public Object
	{
		UC_OBJECT(Random, Object)
	public:
		virtual uint32_t next() = 0;
		virtual float next_float_01() = 0;

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
			return next_float_01() * delta + min;
		}

		template<typename T>
		UC_NODISCARD T range(const Range<T>& value)
		{
			return range(value.min, value.max);
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		T value_limit()
		{
			return static_cast<T>(Random::range(
				std::numeric_limits<T>::min(),
				std::numeric_limits<T>::max()));
		}

		// TODO: random Radians
		// TODO: random Degrees

		template<typename T = Byte>
		Color3<T> color3()
		{
			return {
				range(Color3<T>::MinValue, Color3<T>::MaxValue),
				range(Color3<T>::MinValue, Color3<T>::MaxValue),
				range(Color3<T>::MinValue, Color3<T>::MaxValue)
			};
		}

		Color3b color3b() { return color3<Byte>(); }
		Color3f color3f() { return color3<Float>(); }

		template<typename T>
		Color4<T> color4(bool random_alpha = false)
		{
			return {
				range(Color4<T>::MinValue, Color4<T>::MaxValue),
				range(Color4<T>::MinValue, Color4<T>::MaxValue),
				range(Color4<T>::MinValue, Color4<T>::MaxValue),
				!random_alpha ? Color4<T>::MaxValue : range(Color4<T>::MinValue, Color4<T>::MaxValue)
			};
		}

		Color4b color4b(bool random_alpha = false) { return color4<Byte>(random_alpha); }
		Color4f color4f(bool random_alpha = false) { return color4<float>(random_alpha); }
	};

	class DefaultRandom : public Random
	{
	public:
		DefaultRandom() = default;

		uint32_t next() override;
		float next_float_01() override;

	protected:
		std::random_device _rd;
	};
}