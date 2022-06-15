#include "unicore/StreamLogger.hpp"

namespace unicore
{
	void StreamLogger::write(LogType type, const StringView text)
	{
		_writer.write(type_to_str(type));
		_writer.write(" ");
		_writer.write(text);
		_writer.write("\n");
	}
}
