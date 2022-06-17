#include "unicore/StreamLogger.hpp"

namespace unicore
{
	StreamLogger::StreamLogger(const Shared<WriteStream>& stream)
		: _stream(stream), _writer(*stream.get())
	{}

	void StreamLogger::write(LogType type, const StringView text)
	{
		_writer.write(type_to_str(type));
		_writer.write(" ");
		_writer.write(text);
		_writer.write("\n");
	}
}