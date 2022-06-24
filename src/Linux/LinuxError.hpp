#pragma once
#include "unicore/Defs.hpp"
#if defined(UNICORE_PLATFORM_LINUX)

namespace unicore
{
	class LinuxError
	{
	public:
		explicit LinuxError(int code) : _code(code) {}

		UC_NODISCARD constexpr int code() const { return _code; }
		UC_NODISCARD StringView get_text() const;

		UC_NODISCARD bool is_success() const;
		UC_NODISCARD bool is_not_found() const;

		static LinuxError get_last();

	protected:
		int _code;
	};

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const LinuxError& error);
}

#endif