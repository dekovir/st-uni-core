#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	// BinaryData /////////////////////////////////////////////////////////////////
	BinaryData::BinaryData(void* data, size_t size, Memory::FreeFunc free)
		: _data(data), _size(size), _free(free)
	{
	}

	BinaryData::BinaryData(MemoryChunk& chunk)
		: _data(nullptr), _size(0), _free(nullptr)
	{
		chunk.swap(&_data, &_size, &_free);
	}

	BinaryData::~BinaryData()
	{
		if (_free)
			_free(_data);

		_data = nullptr;
		_size = 0;
		_free = nullptr;
	}

	size_t BinaryData::get_system_memory_use() const
	{
		return sizeof(BinaryData) + _size;
	}

	// DynamicBinaryData //////////////////////////////////////////////////////////
	DynamicBinaryData::DynamicBinaryData(size_t size)
		: _data(size)
	{}

	DynamicBinaryData::DynamicBinaryData(size_t size, Byte default_value)
		: _data(size, default_value)
	{}

	size_t DynamicBinaryData::get_system_memory_use() const
	{
		return sizeof(DynamicBinaryData) + _data.size();
	}
}