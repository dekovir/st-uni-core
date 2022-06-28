#pragma once
#include "unicore/Logger.hpp"

namespace unicore
{
	class GenericLogger : public Logger
	{
	public:
		void write(LogType type, const StringView text) override;
	};
}