#include "unicore/TimeSpan.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	LogHelper& operator<<(LogHelper& helper, const TimeSpan& value)
	{
		return helper << value.total_milliseconds() << "ms";
	}
}