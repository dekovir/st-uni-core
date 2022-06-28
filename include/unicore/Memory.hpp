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

	template<typename T = uint8_t>
	class MemoryChunk
	{
	public:
		explicit MemoryChunk(size_t count)
			: _count(count)
		{
			_data = static_cast<T*>(Memory::alloc(total_size()));
		}

		~MemoryChunk()
		{
			Memory::free(_data);
		}

		UC_NODISCARD T* data() { return _data; }
		UC_NODISCARD const T* data() const { return _data; }

		UC_NODISCARD constexpr size_t count() const { return _count; }
		UC_NODISCARD constexpr size_t total_size() const { return _count * sizeof(T); }

	protected:
		T* _data;
		size_t _count;
	};
}