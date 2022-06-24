#pragma once
#include "unicore/Defs.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinDefs.hpp"

namespace unicore
{
	class WinError
	{
	public:
		explicit WinError(DWORD code);

		UC_NODISCARD int code() const { return _code; }
		UC_NODISCARD String get_text() const;

		UC_NODISCARD bool is_error() const;
		UC_NODISCARD bool is_not_found() const;

		static WinError get_last();

	private:
		const DWORD _code;
	};

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const WinError& error);
}
#endif