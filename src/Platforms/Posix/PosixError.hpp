#pragma once
#include "unicore/Defs.hpp"
#if defined(UNICORE_PLATFORM_POSIX)

namespace unicore
{
	class PosixError
	{
	public:
		explicit PosixError(int code) : _code(code) {}

		UC_NODISCARD constexpr int code() const { return _code; }
		UC_NODISCARD StringView get_text() const;

		UC_NODISCARD bool is_success() const;
		UC_NODISCARD bool is_not_found() const;

		static PosixError get_last();

	protected:
		int _code;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const PosixError&);
}

#endif