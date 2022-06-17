#pragma once
#include "unicore/TimeSpan.hpp"

namespace unicore
{
	class DateTime
	{
	public:
		constexpr DateTime() : _data(0) {}
		explicit constexpr DateTime(time_t data) : _data(data) {}

		UC_NODISCARD constexpr time_t data() const { return _data; }

	protected:
		time_t _data;
	};

	static constexpr bool operator==(const DateTime& a, const DateTime& b)
	{
		return a.data() == b.data();
	}

	static constexpr bool operator!=(const DateTime& a, const DateTime& b)
	{
		return a.data() != b.data();
	}

	static constexpr bool operator<(const DateTime& a, const DateTime& b)
	{
		return a.data() < b.data();
	}

	static constexpr bool operator>(const DateTime& a, const DateTime& b)
	{
		return a.data() > b.data();
	}

	static constexpr bool operator>=(const DateTime& a, const DateTime& b)
	{
		return a.data() >= b.data();
	}

	static constexpr bool operator<=(const DateTime& a, const DateTime& b)
	{
		return a.data() <= b.data();
	}
}
