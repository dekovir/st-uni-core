#include "unicore/File.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	bool ReadFile::read(MemoryChunk& chunk, size_t* bytes_read)
	{
		return read(chunk.data(), chunk.size(), bytes_read);
	}

	bool WriteFile::write(const MemoryChunk& chunk, size_t* bytes_written)
	{
		return write(chunk.data(), chunk.size(), bytes_written);
	}

	// FileWriter ////////////////////////////////////////////////////////////////
	FileWriter& FileWriter::write(StringView str)
	{
		stream.write(str.data(), sizeof(char) * str.size());
		return *this;
	}

	FileWriter& FileWriter::write(WStringView str)
	{
		stream.write(str.data(), sizeof(wchar_t) * str.size());
		return *this;
	}
}