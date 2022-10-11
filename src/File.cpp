#include "unicore/File.hpp"
#include "unicore/Memory.hpp"
#include "unicore/BinaryData.hpp"

namespace unicore
{
	bool ReadFile::read(MemoryChunk& chunk, size_t* bytes_read)
	{
		return read(chunk.data(), chunk.size(), bytes_read);
	}

	Shared<BinaryData> ReadFile::as_data()
	{
		const auto s = size();
		seek(0);

		MemoryChunk chunk(s);
		read(chunk);
		return std::make_shared<BinaryData>(std::move(chunk));
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

	FileWriter& FileWriter::write(StringView16 str)
	{
		stream.write(str.data(), sizeof(Char16) * str.size());
		return *this;
	}

	FileWriter& FileWriter::write(StringView32 str)
	{
		stream.write(str.data(), sizeof(Char32) * str.size());
		return *this;
	}
}
