#include "unicore/TimeSpan.hpp"

namespace unicore
{
	const TimeSpan TimeSpan::Zero(std::chrono::duration<uint64_t, std::milli>(0));

	TimeSpan TimeSpan::add(const TimeSpan& ts) const
	{
		return TimeSpan(_data + ts._data);
	}

	TimeSpan TimeSpan::sub(const TimeSpan& ts) const
	{
		return TimeSpan(_data - ts._data);
	}

	uint64_t TimeSpan::total_milliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(_data).count();
	}

	double TimeSpan::total_seconds() const
	{
		return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(_data).count()) / 1000.f;
	}

	bool TimeSpan::operator==(const TimeSpan& other) const
	{
		return _data == other._data;
	}

	bool TimeSpan::operator!=(const TimeSpan& other) const
	{
		return _data != other._data;
	}

	TimeSpan TimeSpan::from_microseconds(uint64_t ms)
	{
		return TimeSpan(std::chrono::duration<uint64_t, std::micro>(ms));
	}

	TimeSpan TimeSpan::from_milliseconds(uint64_t ms)
	{
		return TimeSpan(std::chrono::duration<uint64_t, std::milli>(ms));
	}

	TimeSpan TimeSpan::from_seconds(float seconds)
	{
		return TimeSpan(std::chrono::duration<float>(seconds));
	}

	TimeSpan TimeSpan::from_seconds(double seconds)
	{
		return TimeSpan(std::chrono::duration<double>(seconds));
	}
}