#include "unicore/Logger.hpp"
#include "unicore/Strings.hpp"
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

	LogHelper::LogHelper(Logger& logger, LogType type)
		: _logger(logger), _type(type)
	{
	}

	LogHelper::~LogHelper()
	{
		_logger.write(_type, _text);
	}

	void LogHelper::append(char c)
	{
		const char str[] = { c, 0 };
		append(str);
	}

	void LogHelper::append(wchar_t c)
	{
		const wchar_t str[] = { c, 0 };
		append(str);
	}

	void LogHelper::append(StringView text)
	{
		_text.append(text.data());
	}

	void LogHelper::append(WStringView text)
	{
		_text.append(Strings::to_utf8(text.data()));
	}

	LogHelper& operator<<(LogHelper& helper, bool value)
	{
		return helper << (value ? L"true" : L"false");
	}

#if defined (_HAS_EXCEPTIONS)
	LogHelper& operator<<(LogHelper& helper, const std::exception& ex)
	{
		return helper << ex.what();
	}
#endif

	LogHelper& operator<<(LogHelper& helper, const char value)
	{
		helper.append(value);
		return helper;
	}

	LogHelper& operator<<(LogHelper& helper, const wchar_t value)
	{
		helper.append(value);
		return helper;
	}

	LogHelper& operator<<(LogHelper& helper, const char* value)
	{
		if (value != nullptr)
			helper.append(value);
		return helper;
	}

	LogHelper& operator<<(LogHelper& helper, const wchar_t* value)
	{
		if (value != nullptr)
			helper.append(value);
		return helper;
	}

	LogHelper& operator<<(LogHelper& helper, const DebugSource& source)
	{
		// TODO: Replace with constexpr
#if defined (UNICORE_PLATFORM_WINDOWS) || defined(UNICORE_PLATFORM_ANDROID)
		const auto* pos = strrchr(source.file.data(), '\\');
#else
		const auto* pos = strrchr(source.file.data(), '/');
#endif
		if (pos != nullptr)
			helper.append(pos + 1);
		else helper.append(source.file);
		return helper << ":" << source.line;
	}

	LogHelper& operator<<(LogHelper& helper, const DebugFunction& source)
	{
		return helper << source._func << " - ";
	}
}