#pragma once
#include "unicore/TimeSpan.hpp"
#include <chrono>

namespace unicore
{
	class Timer
	{
	public:
		using ClockType = std::chrono::high_resolution_clock;

		explicit constexpr Timer(const ClockType::time_point data) noexcept
			: _data(data) {}

		static Timer now() { return Timer(ClockType::now()); }

		UC_NODISCARD constexpr ClockType::time_point data() const { return _data; }

	protected:
		ClockType::time_point _data;
	};

	static constexpr TimeSpan operator-(const Timer& a, const Timer& b)
	{
		const auto duration = a.data() - b.data();
		return TimeSpan::from_duration(duration);
	}
}