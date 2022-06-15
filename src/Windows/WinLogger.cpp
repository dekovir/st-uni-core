#include "WinLogger.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Windows.h>
#include <debugapi.h>
#include <stdio.h>

namespace unicore
{
	void WinLogger::write(LogType type, const StringView text)
	{
		char BUFFER[1024];
		sprintf(BUFFER, "%s %s\n", type_to_str(type), text.data());
		OutputDebugStringA(BUFFER);
	}
}
#endif