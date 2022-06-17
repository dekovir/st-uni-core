#include "unicore/StreamWriter.hpp"

namespace unicore
{
	StreamWriter::StreamWriter(WriteStream& stream)
		: _stream(stream)
	{
	}

	StreamWriter& StreamWriter::write(const StringView text)
	{
		_stream.write(text.data(), sizeof(char), text.size());
		return *this;
	}

	StreamWriter& StreamWriter::write(const WStringView text)
	{
		_stream.write(text.data(), sizeof(wchar_t), text.size());
		return *this;
	}
}