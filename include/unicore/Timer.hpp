#pragma once
#include "unicore/TimeSpan.hpp"
#include <ctime>

namespace unicore
{
	class Timer
	{
	public:
		explicit constexpr Timer(clock_t data) noexcept
			: _data(data) {}

		static Timer now() { return Timer(clock()); }

		UC_NODISCARD constexpr clock_t data() const { return _data; }

	protected:
		clock_t _data;
	};

	static constexpr TimeSpan operator-(const Timer& a, const Timer& b)
	{
		const double delta = a.data() - b.data();
		return TimeSpan::from_seconds(delta / CLOCKS_PER_SEC);
	}
}