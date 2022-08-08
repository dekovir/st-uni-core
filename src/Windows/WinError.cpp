#include "WinError.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "unicore/Strings.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	WinError::WinError(DWORD code)
		: _code(code)
	{
	}

	String WinError::get_text() const
	{
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&lpMsgBuf),
			0, nullptr);

		String error(static_cast<LPCSTR>(lpMsgBuf));
		LocalFree(lpMsgBuf);

		return error;
	}

	bool WinError::is_error() const
	{
		return _code != ERROR_SUCCESS;
	}

	bool WinError::is_not_found() const
	{
		return
			(_code == ERROR_PATH_NOT_FOUND) ||
			(_code == ERROR_FILE_NOT_FOUND) ||
			(_code == ERROR_INVALID_NAME);
	}

	WinError WinError::get_last()
	{
		return WinError(::GetLastError());
	}

	UNICODE_STRING_BUILDER_FORMAT(const WinError&)
	{
		return builder << value.get_text() << L":" << value.code();
	}
}
#endif