#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	void* Memory::alloc(size_t size)
	{
		return new uint8_t[size];
	}

	void Memory::free(void* ptr)
	{
		delete[] static_cast<uint8_t*>(ptr);
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

	// ===========================================================================
	MemoryChunk::MemoryChunk()
		: _data(nullptr), _size(0), _free_data(false)
	{
	}

	MemoryChunk::MemoryChunk(size_t size)
		: _size(size), _free_data(true)
	{
		_data = size > 0 ? static_cast<uint8_t*>(Memory::alloc(size)) : nullptr;
	}

	MemoryChunk::MemoryChunk(uint8_t* data, size_t size, bool free_data)
		: _data(data), _size(size), _free_data(free_data)
	{
	}

	MemoryChunk::MemoryChunk(const MemoryChunk& other)
		: MemoryChunk(other.size())
	{
		Memory::copy(_data, other.data(), _size);
	}

	MemoryChunk::~MemoryChunk()
	{
		free();
	}

	MemoryChunk& MemoryChunk::operator=(const MemoryChunk& other)
	{
		if (this != &other)
		{
			free();

			resize(other.size(), false);
			Memory::copy(_data, other._data, _size);
		}

		return *this;
	}

	MemoryChunk& MemoryChunk::operator=(MemoryChunk&& other) noexcept
	{
		if (this != &other)
		{
			_data = std::exchange(other._data, nullptr);
			_size = std::exchange(other._size, 0);
		}
		return *this;
	}

	void MemoryChunk::clear()
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
			if (_free_data)
				Memory::free(_data);

			_data = nullptr;
			_size = 0;
		}
	}

	// ===========================================================================
	UNICODE_STRING_BUILDER_FORMAT(const MemorySize&)
	{
		constexpr auto KB = 1024;
		constexpr auto MB = KB * 1024;

		if (value.bytes > MB)
		{
			const auto amount = static_cast<float>(value.bytes) / MB;
			builder << amount << "Mb";
		}
		else if (value.bytes > KB)
		{
			const auto amount = static_cast<float>(value.bytes) / KB;
			builder << amount << "Kb";
		}
		else
		{
			builder << value.bytes << "b";
		}
		return builder;
	}
}