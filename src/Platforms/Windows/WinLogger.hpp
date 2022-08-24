#pragma once
#include "unicore/Logger.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)

namespace unicore
{
	class WinLogger : public Logger
	{
		UC_OBJECT(WinLogger, Logger)
	public:
		WinLogger() = default;

		void write(LogType type, const StringView text) override;
	};
}
#endif