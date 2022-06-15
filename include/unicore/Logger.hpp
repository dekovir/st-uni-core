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

	class Logger : public Module
	{
	public:
		virtual void write(LogType type, const StringView text) = 0;

		inline void info(const StringView text) { write(LogType::Info, text); }
		inline void debug(const StringView text) { write(LogType::Debug, text); }
		inline void warn(const StringView text) { write(LogType::Warning, text); }
		inline void err(const StringView text) { write(LogType::Error, text); }

		static Shared<Logger> create();

	protected:
		static const char* type_to_str(LogType type);
	};

	class PrintLogger : public Logger
	{
	public:
		void write(LogType type, const StringView text) override;
	};

	/*class ProxyLogger : public Logger
	{
	public:
		ProxyLogger(const StringView prefix, const Shared<Logger>& logger)
			: _prefix(prefix), _logger(logger)
		{}

		void write(LogType type, const StringView text) override
		{
		}

	protected:
		String _prefix;
		Shared<Logger> _logger;
	};*/

	class MultiLogger : public Logger
	{
	public:
		explicit MultiLogger(std::initializer_list<std::shared_ptr<Logger>> loggers);

		void write(LogType type, const StringView text) override;

	protected:
		List<Shared<Logger>> _loggers;
	};
}