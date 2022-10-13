#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Memory
	{
	public:
		typedef void*(*AllocFunc)(size_t);
		typedef void(*FreeFunc)(void*);

		static void* alloc(size_t size);
		static void free(void* ptr);

		static void set(void* dest, int value, size_t size);
		static void copy(void* dest, const void* src, size_t size);
		static void move(void* dest, const void* src, size_t size);

		static int compare(const void* ptr1, const void* ptr2, size_t size);
		static bool equals(const void* ptr1, const void* ptr2, size_t size);
	};

#define UC_ALLOC(size) Memory::alloc(size)
#define UC_FREE(ptr) Memory::free(ptr)

	class MemoryView
	{
	public:
		MemoryView()
			: _data(nullptr), _size(0) {}
		MemoryView(void* data, size_t size)
			: _data(data), _size(size) {}

		UC_NODISCARD constexpr size_t size() const { return _size; }
		UC_NODISCARD constexpr bool empty() const { return _size == 0; }
		UC_NODISCARD constexpr void* data() const { return _data; }

	protected:
		void* _data;
		size_t _size;
	};

	class MemoryChunk
	{
	public:
		MemoryChunk();
		explicit MemoryChunk(size_t size);
		explicit MemoryChunk(void* data, size_t size, Memory::FreeFunc free);

		MemoryChunk(const MemoryChunk& other);

		constexpr MemoryChunk(MemoryChunk&& other) noexcept
			: _data(other._data), _size(other._size), _free(other._free)
		{
			other._data = nullptr;
			other._size = 0;
			other._free = nullptr;
		}

		~MemoryChunk();

		MemoryChunk& operator=(const MemoryChunk& other);
		MemoryChunk& operator=(MemoryChunk&& other) noexcept;

		UC_NODISCARD constexpr size_t size() const { return _size; }
		UC_NODISCARD constexpr bool empty() const { return _size == 0; }

		UC_NODISCARD constexpr void* data() { return _data; }
		UC_NODISCARD constexpr const void* data() const { return _data; }

		UC_NODISCARD MemoryChunk clone() const;

		void set(uint8_t value = 0);

		void swap(void** data, size_t* size, Memory::FreeFunc* free);

	protected:
		void* _data;
		size_t _size;
		Memory::FreeFunc _free;

		void internal_free();
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const MemoryChunk&);

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

	extern UNICODE_STRING_BUILDER_FORMAT(const MemorySize&);
}