#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Memory
	{
	public:
		static void* alloc(size_t size);
		static void free(void* ptr);
	};

	class MemoryChunk
	{
	public:
		explicit MemoryChunk(size_t size)
			: _size(size)
		{
			_data = static_cast<uint8_t*>(Memory::alloc(size));
		}

		~MemoryChunk()
		{
			Memory::free(_data);
		}

		UC_NODISCARD uint8_t* data() { return _data; }
		UC_NODISCARD const uint8_t* data() const { return _data; }

		UC_NODISCARD constexpr size_t size() const { return _size; }

	protected:
		uint8_t* _data;
		size_t _size;
	};

	struct MemorySize
	{
		size_t bytes;

		MemorySize& operator+=(const MemorySize other)
		{
			bytes += other.bytes;
			return *this;
		}
	};

	static constexpr MemorySize operator+(const MemorySize a, const MemorySize b)
	{
		return { a.bytes + b.bytes };
	}

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const MemorySize& value);
}