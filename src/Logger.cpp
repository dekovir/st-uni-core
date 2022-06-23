#include "unicore/Logger.hpp"
#include "Windows/WinLogger.hpp"
#include <cstdio>

namespace unicore
{
	void PrintLogger::write(LogType type, const StringView text)
	{
		printf("%s %s\n", type_to_str(type), text.data());
	}

	Logger& Logger::get_native()
	{
#if defined(UNICORE_PLATFORM_WINDOWS)
		static WinLogger logger;
#else
		static PrintLogger logger;
#endif
		return logger;
	}

	const char* Logger::type_to_str(LogType type)
	{
		switch (type)
		{
		case LogType::Info: return "i";
		case LogType::Debug: return "d";
		case LogType::Warning: return "w";
		case LogType::Error: return "e";
		}

		UC_ASSERT_ALWAYS_MSG("Invalid LogType");
		return "";
	}
}