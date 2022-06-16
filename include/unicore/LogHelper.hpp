#pragma once
#include "unicore/Logger.hpp"

namespace unicore
{
	class LogHelper
	{
	public:
		LogHelper(Logger* logger, LogType type);
		~LogHelper();

		LogHelper& operator <<(bool value);

#if defined (_HAS_EXCEPTIONS)
		LogHelper& operator <<(const std::exception& ex);
#endif

		LogHelper& operator << (const char value) { append(value); return *this; }
		LogHelper& operator << (const wchar_t value) { append(value); return *this; }

		LogHelper& operator << (const char* value) { append(value); return *this; }
		LogHelper& operator << (const wchar_t* value) { append(value); return *this; }

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
			return { logger.get(), type };
		}

		static LogHelper create(Logger* logger, LogType type)
		{
			return { logger, type };
		}

		static LogHelper create(Logger& logger, LogType type)
		{
			return { &logger, type };
		}

		struct DebugSource
		{
			DebugSource(const char* _file, int _line)
				: file(_file), line(_line)
			{}

			const char* file;
			const int line;
		};

		struct DebugFunction
		{
			DebugFunction(const char* func) : _func(func) {}

			const char* _func;
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
		Logger* const _logger;
		const LogType _type;
		String _text;
	};
}

#define UNICORE_LOG_INFO(logger) \
		LogHelper::create(logger, LogType::Info)

#define UNICORE_LOG_DEBUG(logger) \
		if (LogHelper::allow(LogType::Debug)) \
				LogHelper::create(logger, LogType::Debug) \
				<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "

#define UNICORE_LOG_WARNING(logger) \
		LogHelper::create(logger, LogType::Warning) \
		<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "

#define UNICORE_LOG_ERROR(logger) \
		LogHelper::create(logger, LogType::Error) \
		<< LogHelper::DebugSource(__FILE__, __LINE__) << L" "