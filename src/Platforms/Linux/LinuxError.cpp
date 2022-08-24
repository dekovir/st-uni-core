#include "LinuxError.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "unicore/Logger.hpp"

namespace unicore
{
	StringView LinuxError::get_text() const
	{
		return strerror(_code);
	}

	bool LinuxError::is_success() const
	{
		return _code == 0;
	}

	bool LinuxError::is_not_found() const
	{
		return _code == ENOENT;
	}

	LinuxError LinuxError::get_last()
	{
		return LinuxError(errno);
	}

	UNICODE_STRING_BUILDER_FORMAT(const LinuxError&)
	{
		return builder << value.get_text() << ":" << value.code();
	}
}

#endif