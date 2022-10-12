#include "unicore/io/FileLogger.hpp"

namespace unicore
{
	FileLogger::FileLogger(const Shared<WriteFile>& file)
		: _file(file), _writer(*file.get())
	{}

	void FileLogger::write(LogType type, const StringView text)
	{
		_writer.write(type_to_str(type));
		_writer.write(" ");
		_writer.write(text);
		_writer.write("\n");
	}
}