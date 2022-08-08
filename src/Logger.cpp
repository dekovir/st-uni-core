#include "unicore/Logger.hpp"
#include "unicore/Unicode.hpp"
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

	UNICODE_STRING_BUILDER_FORMAT(const DebugSource&)
	{
		// TODO: Replace with constexpr
#if defined (UNICORE_PLATFORM_WINDOWS) || defined(UNICORE_PLATFORM_ANDROID)
		const auto* pos = strrchr(value.file.data(), '\\');
#else
		const auto* pos = strrchr(value.file.data(), '/');
#endif
		if (pos != nullptr)
			builder.append(pos + 1);
		else builder.append(value.file);
		return builder << ":" << value.line;
	}

	UNICODE_STRING_BUILDER_FORMAT(const DebugFunction&)
	{
		return builder << value._func << " - ";
	}
}