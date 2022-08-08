#include "WinLogger.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Windows.h>
#include <debugapi.h>
#include <cstdio>

namespace unicore
{
	void WinLogger::write(LogType type, const StringView text)
	{
		const auto str = StringBuilder::format("{} {}\n", type_to_str(type), text);
		OutputDebugStringA(str.data());
	}
}
#endif