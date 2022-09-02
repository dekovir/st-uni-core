#include "PosixError.hpp"
#if defined(UNICORE_PLATFORM_POSIX)
#include "unicore/Logger.hpp"

namespace unicore
{
	StringView PosixError::get_text() const
	{
		return strerror(_code);
	}

	bool PosixError::is_success() const
	{
		return _code == 0;
	}

	bool PosixError::is_not_found() const
	{
		return _code == ENOENT;
	}

	PosixError PosixError::get_last()
	{
		return PosixError(errno);
	}

	UNICODE_STRING_BUILDER_FORMAT(const PosixError&)
	{
		return builder << value.get_text() << ":" << value.code();
	}
}

#endif