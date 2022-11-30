#include "unicore/system/Memory.hpp"
#include "unicore/math/Math.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	// TODO: Debug leak detection
	void* Memory::alloc(size_t size)
	{
		return std::malloc(size);
	}

	void* Memory::realloc(void* ptr, size_t size)
	{
		return std::realloc(ptr, size);
	}

	void Memory::free(void* ptr)
	{
		std::free(ptr);
	}

	void Memory::set(void* dest, int value, size_t size)
	{
		std::memset(dest, value, size);
	}

	void Memory::copy(void* dest, const void* src, size_t size)
	{
		std::memcpy(dest, src, size);
	}

	void Memory::move(void* dest, const void* src, size_t size)
	{
		std::memmove(dest, src, size);
	}

	int Memory::compare(const void* ptr1, const void* ptr2, size_t size)
	{
		return std::memcmp(ptr1, ptr2, size);
	}

	bool Memory::equals(const void* ptr1, const void* ptr2, size_t size)
	{
		return compare(ptr1, ptr2, size) == 0;
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
		internal_free();
	}

	MemoryChunk& MemoryChunk::operator=(const MemoryChunk& other)
	{
		if (this != &other)
		{
			// TODO: Optimize (if same size)
			internal_free();

			_data = Memory::alloc(other.size());
			_size = other.size();

			Memory::copy(_data, other._data, _size);
		}

		return *this;
	}

	MemoryChunk& MemoryChunk::operator=(MemoryChunk&& other) noexcept
	{
		if (this != &other)
		{
			internal_free();

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

	void MemoryChunk::set(uint8_t value)
	{
		if (!empty())
			Memory::set(_data, value, _size);
	}

	void MemoryChunk::swap(void** data, Size* size, Memory::FreeFunc* free)
	{
		if (_data) std::swap(_data, *data);
		if (_size) std::swap(_size, *size);
		if (_free) std::swap(_free, *free);
	}

	void MemoryChunk::internal_free()
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