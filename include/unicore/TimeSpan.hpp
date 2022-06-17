#pragma once
#include "unicore/Defs.hpp"
#include <chrono>

namespace unicore
{
	class TimeSpan
	{
	public:
		TimeSpan() = default;

		template<typename TRep, typename TPeriod>
		explicit TimeSpan(std::chrono::duration<TRep, TPeriod> duration)
		{
			_data = std::chrono::duration_cast<decltype(_data)>(duration);
		}

		UC_NODISCARD TimeSpan add(const TimeSpan& ts) const;
		UC_NODISCARD TimeSpan sub(const TimeSpan& ts) const;

		UC_NODISCARD uint64_t total_milliseconds() const;
		UC_NODISCARD double total_seconds() const;

		TimeSpan operator + (const TimeSpan& ts) const { return add(ts); }
		TimeSpan operator - (const TimeSpan& ts) const { return sub(ts); }

		TimeSpan& operator += (const TimeSpan& ts)
		{
			_data += ts._data;
			return *this;
		}

		bool operator == (const TimeSpan& other) const;
		bool operator != (const TimeSpan& other) const;

		static TimeSpan from_microseconds(uint64_t ms);
		static TimeSpan from_milliseconds(uint64_t ms);

		static TimeSpan from_seconds(float seconds);
		static TimeSpan from_seconds(double seconds);

		static const TimeSpan Zero;

	protected:
		std::chrono::nanoseconds _data;
	};
}
