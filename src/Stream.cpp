#include "unicore/Stream.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	bool ReadStream::read(MemoryChunk& chunk, size_t* bytes_read)
	{
		return read(chunk.data(), chunk.size(), bytes_read);
	}

	bool WriteStream::write(const MemoryChunk& chunk, size_t* bytes_written)
	{
		return write(chunk.data(), chunk.size(), bytes_written);
	}

	// StreamWriter ///////////////////////////////////////////////////////////////
	StreamWriter& StreamWriter::write(StringView str)
	{
		stream.write(str.data(), sizeof(char) * str.size());
		return *this;
	}

	StreamWriter& StreamWriter::write(WStringView str)
	{
		stream.write(str.data(), sizeof(wchar_t) * str.size());
		return *this;
	}
}