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

	LogHelper& LogHelper::operator<<(bool value)
	{
		append(value ? L"true" : L"false");
		return *this;
	}

	LogHelper& LogHelper::operator<<(const char* value)
	{
		if (value != nullptr)
			append(value);

		return *this;
	}

	LogHelper& LogHelper::operator<<(const wchar_t* value)
	{
		if (value != nullptr)
			append(value);

		return *this;
	}

#if defined (_HAS_EXCEPTIONS)
	LogHelper& LogHelper::operator<<(const std::exception& ex)
	{
		append(ex.what());
		return *this;
	}
#endif

	LogHelper& LogHelper::operator<<(const DebugSource& source)
	{
		// TODO: Replace with constexpr
#if defined (UNICORE_PLATFORM_WINDOWS) || defined(UNICORE_PLATFORM_ANDROID)
		const auto* pos = strrchr(source.file.data(), '\\');
#else
		const auto* pos = strrchr(source.file.data(), '/');
#endif
		if (pos != nullptr)
			append(pos + 1);
		else append(source.file);
		append(L":");
		this->operator<<(source.line);

		return *this;
	}

	LogHelper& LogHelper::operator<<(const DebugFunction& source)
	{
		this->operator<<(source._func);
		this->operator<<(L" - ");
		return *this;
	}

	void LogHelper::append(const char c)
	{
		const char str[] = { c, 0 };
		append(str);
	}

	void LogHelper::append(const wchar_t c)
	{
		const wchar_t str[] = { c, 0 };
		append(str);
	}

	void LogHelper::append(const StringView text)
	{
		_text.append(text.data());
	}

	void LogHelper::append(const WStringView text)
	{
		_text.append(Strings::to_utf8(text.data()));
	}

	LogHelper& operator<<(LogHelper& helper, const TypeInfo& type)
	{
		return helper << '<' << type.name() << '>';
	}

	LogHelper& operator<<(LogHelper& helper, const TypeIndex& type)
	{
		return helper << '<' << type.name() << '>';
	}
}