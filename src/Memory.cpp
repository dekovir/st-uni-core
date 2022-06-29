#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"

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