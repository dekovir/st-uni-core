#pragma once
#include "unicore/Debug.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinDefs.hpp"

namespace unicore
{
	struct WinError : Error<DWORD>
	{
		explicit WinError(DWORD code)
			: Error<DWORD>(code)
		{
		}

		UC_NODISCARD String get_text() const;

		UC_NODISCARD bool is_error() const;
		UC_NODISCARD bool is_not_found() const;

		static WinError get_last();
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const WinError&);
}
#endif