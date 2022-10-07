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

		const auto str = StringBuilder::format(U"{} {}\n", type_to_str(type), text);
		const auto wcs = Unicode::to_wcs(str);
		OutputDebugStringW(wcs.data());
		_prev = text;
	}
}
#endif