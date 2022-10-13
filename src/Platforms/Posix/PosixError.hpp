#pragma once
#include "unicore/system/Debug.hpp"
#if defined(UNICORE_PLATFORM_POSIX)

namespace unicore
{
	struct PosixError : Error<int>
	{
		explicit PosixError(int code)
			: Error<int>(code)
		{
		}

		UC_NODISCARD StringView get_text() const;

		UC_NODISCARD bool is_success() const;
		UC_NODISCARD bool is_not_found() const;

		static PosixError get_last();
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const PosixError&);
}

#endif