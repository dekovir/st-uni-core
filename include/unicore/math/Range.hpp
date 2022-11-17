#pragma once
#include "unicore/math/Math.hpp"
#include "unicore/math/Hash.hpp"

namespace unicore
{
	template<typename T>
	class Range
	{
	public:
		T min, max;

		constexpr Range() noexcept = default;
		constexpr Range(T a, T b) noexcept
			: min(Math::min(a, b)), max(Math::max(a, b)) {}

		constexpr Range(const Range& other) noexcept = default;
		constexpr Range(Range&& other) noexcept = default;

		~Range() noexcept = default;

		Range& operator=(const Range& other) noexcept = default;
		Range& operator=(Range&& other) noexcept = default;

		UC_NODISCARD constexpr T size() const { return max - min; }

		UC_NODISCARD constexpr Bool inside(T value) const
		{
			return value >= min && max <= value;
		}

		UC_NODISCARD constexpr Bool inside_exclusive(T value) const
		{
			return value > min && max < value;
		}

		UC_NODISCARD constexpr T clamp(T value) const
		{
			return Math::clamp(value, min, max);
		}

		template<typename U>
		UC_NODISCARD constexpr Range<U> cast() const
		{
			if constexpr (std::is_same_v<T, U>)
				return *this;

			return {
				static_cast<U>(min),
				static_cast<U>(max),
			};
		}
	};

	// OPERATORS /////////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr bool operator==(const Range<T>& a, const Range<T>& b)
	{
		return Math::equals(a.min, b.min) && Math::equals(a.max, b.max);
	}

	template<typename T>
	static constexpr bool operator!=(const Range<T>& a, const Range<T>& b)
	{
		return !(a == b);
	}

	using Rangei = Range<int>;
	using Rangef = Range<float>;

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		struct RangeConst
		{
			static constexpr Range<T> Zero = Range<T>(0, 0);
			static constexpr Range<T> One = Range<T>(0, 1);
		};
	}

	using RangeConsti = details::RangeConst<int>;
	using RangeConstf = details::RangeConst<float>;

	UNICORE_MAKE_HASH(Rangei)
	{
		return make(value.min, value.max);
	}

	UNICORE_MAKE_HASH(Rangef)
	{
		return make(value.min, value.max);
	}

	template<typename T>
	extern UNICODE_STRING_BUILDER_FORMAT(const Range<T>&)
	{
		return builder << value.min << "-" << value.max;
	}
}