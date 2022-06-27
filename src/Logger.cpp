#include "unicore/Logger.hpp"
#include <cstdio>

namespace unicore
{
	void PrintLogger::write(LogType type, const StringView text)
	{
		printf("%s %s\n", type_to_str(type), text.data());
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

	ProxyLogger::ProxyLogger(const StringView prefix, Logger& logger)
		: _prefix(prefix), _logger(logger)
	{}

	void ProxyLogger::write(LogType type, const StringView text)
	{
		String str;
		str.reserve(_prefix.size() + text.size());
		str += _prefix;
		str += text;
		_logger.write(type, str);
	}
}