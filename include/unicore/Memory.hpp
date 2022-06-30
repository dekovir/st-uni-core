#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Memory
	{
	public:
		static void* alloc(size_t size);
		static void free(void* ptr);

		static void set(void* dest, int value, size_t size);
		static void copy(void* dest, const void* src, size_t size);
		static void move(void* dest, const void* src, size_t size);
	};

	class MemoryChunk
	{
	public:
		explicit MemoryChunk(size_t size);

		MemoryChunk(const MemoryChunk& other);

		constexpr MemoryChunk(MemoryChunk&& other) noexcept
			: _data(other._data), _size(other._size)
		{
			other._data = nullptr;
			other._size = 0;
		}

		~MemoryChunk();

		MemoryChunk& operator=(const MemoryChunk& other);
		MemoryChunk& operator=(MemoryChunk&& other) noexcept;

		UC_NODISCARD constexpr size_t size() const { return _size; }
		UC_NODISCARD constexpr bool empty() const { return _size == 0; }

		UC_NODISCARD constexpr uint8_t* data() { return _data; }
		UC_NODISCARD constexpr const uint8_t* data() const { return _data; }

		void clear();

		void resize(size_t new_size, bool copy_data = true);
		void free();

	protected:
		uint8_t* _data;
		size_t _size;
	};

	static constexpr bool operator==(const MemoryChunk& a, const MemoryChunk& b)
	{
		return a.data() == b.data();
	}

	static constexpr bool operator!=(const MemoryChunk& a, const MemoryChunk& b)
	{
		return !(a == b);
	}

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