#include "WinLogger.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Windows.h>
#include <debugapi.h>

namespace unicore
{
	void WinLogger::write(LogType type, const StringView text)
	{
		if (_prev == text)
			return;

		const auto wcs = StringBuilder::format(L"{} {}\n", type_to_str(type), text);
		OutputDebugStringW(wcs.data());
		_prev = text;
	}
}
#endif