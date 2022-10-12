#pragma once
#include "unicore/Module.hpp"
#include "unicore/Debug.hpp"
#include "unicore/StringBuilder.hpp"

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

	class LogHelper : public StringBuilder
	{
	public:
		LogHelper(Logger& logger, LogType type);
		~LogHelper();

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
	};
}

#define UC_LOG_TYPE(logger, type) \
		LogHelper::create(logger, type) << LogHelper::LValueFix{}

#define UC_LOG_INFO(logger) \
		UC_LOG_TYPE(logger, LogType::Info)

#define UC_LOG_DEBUG(logger) \
	UC_LOG_TYPE(logger, LogType::Debug) << UC_DEBUG_SOURCE() << ' '

#define UC_LOG_WARNING(logger) \
		UC_LOG_TYPE(logger, LogType::Warning) << UC_DEBUG_SOURCE() << ' '

#define UC_LOG_ERROR(logger) \
		UC_LOG_TYPE(logger, LogType::Error) << UC_DEBUG_SOURCE() << ' '