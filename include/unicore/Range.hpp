#pragma once
#include "unicore/Math.hpp"
#include "unicore/Random.hpp"

namespace unicore
{
	template<typename T>
	class Range
	{
	public:
		T min, max;

		constexpr Range() = default;
		constexpr Range(T a, T b)
			: min(Math::min(a, b)), max(Math::max(a, b)) {}

		constexpr Range(const Range& other) = default;
		constexpr Range(Range&& other) noexcept = default;

		~Range() = default;

		Range& operator=(const Range& other) = default;
		Range& operator=(Range&& other) noexcept = default;

		UC_NODISCARD constexpr T size() const { return max - min; }

		UC_NODISCARD T random(Random& rnd) const { return rnd.range(min, max); }
	};

	template<typename T>
	static constexpr bool operator==(const Range<T>& a, const Range<T>& b)
	{
		return a.min == b.min && a.max == b.max;
	}

	template<typename T>
	static constexpr bool operator!=(const Range<T>& a, const Range<T>& b)
	{
		return !(a == b);
	}
}