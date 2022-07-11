#pragma once
#include "unicore/Logger.hpp"

namespace unicore
{
	class GenericLogger : public Logger
	{
		UC_OBJECT(GenericLogger, Logger)
	public:
		void write(LogType type, const StringView text) override;
	};
}