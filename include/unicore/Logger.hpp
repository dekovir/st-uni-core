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

		static Logger& get_native();

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
}