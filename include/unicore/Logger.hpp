#pragma once
#include "unicore/Module.hpp"

namespace unicore
{
	enum class LogType
	{
		Info,
		Debug,
		Warning,
		Error,
	};

	class Logger : public Object
	{
		UC_OBJECT(Logger, Object)
	public:
		virtual void write(LogType type, const StringView text) = 0;

		inline void info(const StringView text) { write(LogType::Info, text); }
		inline void debug(const StringView text) { write(LogType::Debug, text); }
		inline void warn(const StringView text) { write(LogType::Warning, text); }
		inline void err(const StringView text) { write(LogType::Error, text); }

	protected:
		static const char* type_to_str(LogType type);
	};

	class MultiLogger : public Logger
	{
	public:
		List<Logger*> list;

		MultiLogger() = default;
		MultiLogger(std::initializer_list<Logger*> args);

		void write(LogType type, const StringView text) override;
	};

	class PrintLogger : public Logger
	{
		UC_OBJECT(PrintLogger, Logger)
	public:
		void write(LogType type, const StringView text) override;
	};

	// TODO: Add format library to implement
	class ProxyLogger : public Logger
	{
	public:
		ProxyLogger(const StringView prefix, Logger& logger);

		void write(LogType type, const StringView text) override;

	protected:
		String _prefix;
		Logger& _logger;
	};

	class LogHelper
	{
	public:
		LogHelper(Logger& logger, LogType type);
		~LogHelper();

		void append(char c);
		void append(wchar_t c);

		void append(StringView text);
		void append(WStringView text);

		struct LValueFix {};

		LogHelper& operator<<(UC_UNUSED LValueFix) { return *this; }

		static LogHelper create(Logger& logger, LogType type)
		{
			return { logger, type };
		}

		static LogHelper create(const std::shared_ptr<Logger>& logger, LogType type)
		{
			return { *logger, type };
		}

		static LogHelper create(Logger* logger, LogType type)
		{
			return { *logger, type };
		}

	protected:
		Logger& _logger;
		const LogType _type;
		String _text;
	};

	LogHelper& operator <<(LogHelper& helper, bool value);

#if defined (_HAS_EXCEPTIONS)
	LogHelper& operator <<(LogHelper& helper, const std::exception& ex);
#endif

	extern LogHelper& operator << (LogHelper& helper, char value);
	extern LogHelper& operator << (LogHelper& helper, wchar_t value);

	extern LogHelper& operator <<(LogHelper& helper, const char* value);
	extern LogHelper& operator <<(LogHelper& helper, const wchar_t* value);

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern LogHelper& operator << (LogHelper& helper, const BasicStringView<TChar> value)
	{
		helper.append(value);
		return helper;
	}

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern LogHelper& operator << (LogHelper& helper, const BasicString<TChar>& value)
	{
		helper.append(value);
		return helper;
	}

	template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
	extern LogHelper& operator<<(LogHelper& helper, T value)
	{
		const auto tmp = static_cast<int>(value);
		const auto str = std::to_wstring(tmp);
		return helper << str.c_str();
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	extern LogHelper& operator<<(LogHelper& helper, T value)
	{
		const auto str = std::to_wstring(value);
		return helper << str.c_str();
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	extern LogHelper& operator<<(LogHelper& helper, T value)
	{
		const auto str = std::to_wstring(value);
		return helper << str.c_str();
	}

	template<typename T>
	extern LogHelper& operator<<(LogHelper& helper, std::initializer_list<T> list)
	{
		int index = 0;
		for (const auto* value : list)
		{
			if (index > 0)
				helper << ',';
			helper << value;
			index++;
		}
		return helper;
	}

	struct DebugSource
	{
		constexpr DebugSource(StringView _file, int _line)
			: file(_file), line(_line)
		{
		}

		const StringView file;
		const int line;

		static DebugSource create(const char* file, int line)
		{
#if defined (UNICORE_PLATFORM_WINDOWS) || defined(UNICORE_PLATFORM_ANDROID)
			const auto pos = std::string_view(file).find_last_of('\\');
#else
			const auto pos = std::string_view(file).find_last_of('/');
#endif
			return { (pos != std::string_view::npos ? &file[pos + 1] : file), line };
		}
	};

	struct DebugFunction
	{
		explicit constexpr DebugFunction(StringView func) : _func(func) {}

		StringView _func;
	};

	extern LogHelper& operator << (LogHelper& helper, const DebugSource& source);
	extern LogHelper& operator << (LogHelper& helper, const DebugFunction& source);
}

#define UC_LOG_TYPE(logger, type) \
		LogHelper::create(logger, type) << LogHelper::LValueFix{}

#define UC_LOG_INFO(logger) \
		UC_LOG_TYPE(logger, LogType::Info)

#define UC_LOG_DEBUG(logger) \
	UC_LOG_TYPE(logger, LogType::Debug) << DebugSource(__FILE__, __LINE__) << ' '

#define UC_LOG_WARNING(logger) \
		UC_LOG_TYPE(logger, LogType::Warning) << DebugSource(__FILE__, __LINE__) << ' '

#define UC_LOG_ERROR(logger) \
		UC_LOG_TYPE(logger, LogType::Error) << DebugSource(__FILE__, __LINE__) << ' '