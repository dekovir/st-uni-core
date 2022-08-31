#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	BinaryData::BinaryData(MemoryChunk chunk)
		: _chunk(std::move(chunk))
	{
	}
}