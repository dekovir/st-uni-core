#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	void* Memory::alloc(size_t size)
	{
		return new char[size];
	}

	void Memory::free(void* ptr)
	{
		delete[] static_cast<char*>(ptr);
	}

	void Memory::set(void* dest, int value, size_t size)
	{
		memset(dest, value, size);
	}

	void Memory::copy(void* dest, const void* src, size_t size)
	{
		memcpy(dest, src, size);
	}

	void Memory::move(void* dest, const void* src, size_t size)
	{
		memmove(dest, src, size);
	}

	void MemoryChunk::reset()
	{
		if (!empty())
			Memory::set(_data, 0, _size);
	}

	void MemoryChunk::resize(size_t new_size, bool copy_data)
	{
		if (new_size == _size) return;

		const auto new_mem = static_cast<uint8_t*>(Memory::alloc(new_size));
		if (copy_data)
		{
			const auto copy_size = Math::min(new_size, _size);
			Memory::copy(new_mem, _data, copy_size);
		}

		_data = new_mem;
		_size = new_size;
	}

	void MemoryChunk::free()
	{
		if (!empty())
		{
			Memory::free(_data);
			_data = nullptr;
			_size = 0;
		}
	}

	LogHelper& operator<<(LogHelper& helper, const MemorySize& value)
	{
		constexpr auto KB = 1024;
		constexpr auto MB = KB * 1024;

		if (value.bytes > MB)
		{
			const auto amount = static_cast<float>(value.bytes) / MB;
			helper << amount << "Mb";
		}
		else if (value.bytes > KB)
		{
			const auto amount = static_cast<float>(value.bytes) / KB;
			helper << amount << "Kb";
		}
		else
		{
			helper << value.bytes << "b";
		}
		return helper;
	}
}