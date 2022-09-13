#include "unicore/MemoryData.hpp"

namespace unicore
{
	// ConstMemoryData ////////////////////////////////////////////////////////////
	ConstMemoryData::ConstMemoryData(const void* data, const size_t size)
		: _data(data), _size(size)
	{}

	// DynamicMemoryData //////////////////////////////////////////////////////////
	DynamicMemoryData::DynamicMemoryData(size_t size)
		: _data(size)
	{}

	DynamicMemoryData::DynamicMemoryData(size_t size, uint8_t value)
		: _data(size, value)
	{}

	void DynamicMemoryData::resize(size_t new_size)
	{
		_data.resize(new_size);
	}

	void DynamicMemoryData::resize(size_t new_size, uint8_t value)
	{
		_data.resize(new_size, value);
	}

	void DynamicMemoryData::fill(uint8_t value)
	{
		std::fill(_data.begin(), _data.end(), value);
	}
}