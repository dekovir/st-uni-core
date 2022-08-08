#include "unicore/TimeSpan.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	UNICODE_STRING_BUILDER_FORMAT(const TimeSpan&)
	{
		return builder << value.total_milliseconds() << "ms";
	}
}