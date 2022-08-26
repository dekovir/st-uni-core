#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	// TODO: Debug leak detection
	void* Memory::alloc(size_t size)
	{
		return new uint8_t[size];
	}

	void Memory::free(void* ptr)
	{
		delete[] static_cast<const uint8_t*>(ptr);
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
		: _data(nullptr), _size(0), _free(nullptr)
	{
	}

	MemoryChunk::MemoryChunk(size_t size)
		: MemoryChunk(Memory::alloc(size), size, &Memory::free)
	{
		_data = size > 0 ? static_cast<uint8_t*>(UC_ALLOC(size)) : nullptr;
	}

	MemoryChunk::MemoryChunk(void* data, size_t size, Memory::FreeFunc free)
		: _data(data), _size(size), _free(free)
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
			free();

			_data = std::exchange(other._data, nullptr);
			_size = std::exchange(other._size, 0);
			_free = std::exchange(other._free, nullptr);
		}
		return *this;
	}

	MemoryChunk MemoryChunk::clone() const
	{
		MemoryChunk clone(_size);
		Memory::copy(clone._data, _data, _size);
		return clone;
	}

	void MemoryChunk::fill(uint8_t value)
	{
		if (!empty())
			Memory::set(_data, value, _size);
	}

	void MemoryChunk::resize(size_t new_size, bool copy_data)
	{
		if (new_size == _size) return;

		const auto new_mem = Memory::alloc(new_size);
		if (copy_data)
		{
			const auto copy_size = Math::min(new_size, _size);
			Memory::copy(new_mem, _data, copy_size);
		}

		free();

		_data = new_mem;
		_size = new_size;
		_free = &Memory::free;
	}

	void MemoryChunk::free()
	{
		if (_free && _data)
			_free(_data);

		_free = nullptr;
		_data = nullptr;
		_size = 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const MemoryChunk&)
	{
		return builder << "{" << value.data() << ", " << MemorySize{ value.size() } << "}";
	}

	// ===========================================================================
	UNICODE_STRING_BUILDER_FORMAT(const MemorySize&)
	{
		constexpr auto KB = 1024;
		constexpr auto MB = KB * 1024;

		if (value.bytes >= MB)
		{
			const auto amount = static_cast<float>(value.bytes) / MB;
			builder << amount << "Mb";
		}
		else if (value.bytes >= KB)
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