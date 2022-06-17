#include "WinLogger.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include <Windows.h>
#include <debugapi.h>
#include <cstdio>

namespace unicore
{
	void WinLogger::write(LogType type, const StringView text)
	{
		// TODO: Replace with format library
		constexpr auto buffer_size = 1024;
		char buffer[buffer_size];

		sprintf_s(buffer, buffer_size, "%s %s\n", type_to_str(type), text.data());
		OutputDebugStringA(buffer);
	}
}
#endif