#include "unicore/DateTime.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	DateTime::DateTime(time_t data) noexcept
		: _data(ClockType::from_time_t(data))
	{}

	time_t DateTime::to_time_t() const noexcept
	{
		return ClockType::to_time_t(_data);
	}

	DateTime DateTime::now() noexcept
	{
		return DateTime(ClockType::now());
	}

	LogHelper& operator<<(LogHelper& helper, const DateTime& time)
	{
		const auto value = time.to_time_t();
		return helper << ctime(&value);
	}
}