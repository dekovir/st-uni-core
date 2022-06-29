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