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
	public:
		virtual void write(LogType type, const StringView text) = 0;

		inline void info(const StringView text) { write(LogType::Info, text); }
		inline void debug(const StringView text) { write(LogType::Debug, text); }
		inline void warn(const StringView text) { write(LogType::Warning, text); }
		inline void err(const StringView text) { write(LogType::Error, text); }

	protected:
		static const char* type_to_str(LogType type);
	};

	class PrintLogger : public Logger
	{
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

		LogHelper& operator <<(bool value);

#if defined (_HAS_EXCEPTIONS)
		LogHelper& operator <<(const std::exception& ex);
#endif

		LogHelper& operator << (const char value) { append(value); return *this; }
		LogHelper& operator << (const wchar_t value) { append(value); return *this; }

		LogHelper& operator << (const char* value);
		LogHelper& operator << (const wchar_t* value);

		LogHelper& operator << (const StringView value) { append(value); return *this; }
		LogHelper& operator << (const WStringView value) { append(value); return *this; }

		LogHelper& operator << (const String& value) { append(value); return *this; }
		LogHelper& operator << (const WString& value) { append(value); return *this; }

		template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
		LogHelper& operator<<(T value)
		{
			const auto tmp = static_cast<int>(value);
			const auto str = std::to_wstring(tmp);
			append(str.c_str());
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		LogHelper& operator<<(T value)
		{
			const auto str = std::to_wstring(value);
			append(str.c_str());
			return *this;
		}

		template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		LogHelper& operator<<(T value)
		{
			const auto str = std::to_wstring(value);
			append(str.c_str());
			return *this;
		}

		template<typename T>
		LogHelper& operator<<(std::initializer_list<T> list)
		{
			int index = 0;
			for (const auto* value : list)
			{
				if (index > 0)
					append(L",");

				this->operator<<(value);
				index++;
			}
			return *this;
		}

		static LogHelper create(const std::shared_ptr<Logger>& logger, LogType type)
		{
			return { *logger, type };
		}

		static LogHelper create(Logger* logger, LogType type)
		{
			return { *logger, type };
		}

		static LogHelper create(Logger& logger, LogType type)
		{
			return { logger, type };
		}

		struct DebugSource
		{
			constexpr DebugSource(StringView _file, int _line)
				: file(_file), line(_line)
			{
			}

			const StringView file;
			const int line;
		};

		struct DebugFunction
		{
			explicit constexpr DebugFunction(StringView func) : _func(func) {}

			StringView _func;
		};

		LogHelper& operator << (const DebugSource& source);
		LogHelper& operator << (const DebugFunction& source);

		void append(const char c);
		void append(const wchar_t c);

		void append(const StringView text);
		void append(const WStringView text);

		static bool allow(LogType type)
		{
#if defined (_DEBUG)
			return true;
#else
			return type != LogType::Debug;
#endif
		}

	protected:
		Logger& _logger;
		const LogType _type;
		String _text;
	};

	extern LogHelper& operator << (LogHelper& helper, const TypeInfo& type);
	extern LogHelper& operator << (LogHelper& helper, const TypeIndex& type);
}

#define UC_LOG_INFO(logger) \
		LogHelper::create(logger, LogType::Info)

#define UC_LOG_DEBUG(logger) \
		if (LogHelper::allow(LogType::Debug)) \
				LogHelper::create(logger, LogType::Debug) \
				<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "

#define UC_LOG_WARNING(logger) \
		LogHelper::create(logger, LogType::Warning) \
		<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "

#define UC_LOG_ERROR(logger) \
		LogHelper::create(logger, LogType::Error) \
		<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "