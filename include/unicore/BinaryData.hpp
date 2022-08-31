#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class BinaryData : public Resource
	{
		UC_OBJECT(BinaryData, Resource)
	public:
		explicit BinaryData(MemoryChunk chunk);

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BinaryData) + _chunk.size(); }

		UC_NODISCARD const void* data() const { return _chunk.data(); }
		UC_NODISCARD size_t size() const { return _chunk.size(); }

	protected:
		MemoryChunk _chunk;
	};
}