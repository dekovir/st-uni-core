#include "GenericLogger.hpp"

namespace unicore
{
	void GenericLogger::write(LogType type, const StringView text)
	{
		printf("%s %s\n", type_to_str(type), text.data());
	}
}