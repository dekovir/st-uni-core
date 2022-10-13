#include "unicore/io/Logger.hpp"
#include "unicore/system/Unicode.hpp"
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

	MultiLogger::MultiLogger(std::initializer_list<Logger*> args)
		: list(args)
	{
	}

	void MultiLogger::write(LogType type, const StringView text)
	{
		for (const auto logger : list)
			logger->write(type, text);
	}

	ProxyLogger::ProxyLogger(const StringView prefix, Logger& logger)
		: _prefix(prefix), _logger(logger)
	{}

	void ProxyLogger::write(LogType type, const StringView text)
	{
		const auto str = StringBuilder::format("{}{}", _prefix, text);
		_logger.write(type, str);
	}

	LogHelper::LogHelper(Logger& logger, LogType type)
		: _logger(logger), _type(type)
	{
	}

	LogHelper::~LogHelper()
	{
		_logger.write(_type, data);
	}
}