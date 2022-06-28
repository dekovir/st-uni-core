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
		if (value.total_bytes > 1024)
		{
			const auto amount = static_cast<float>(value.total_bytes) / 1024;
			helper << amount << "Kb";
		}
		else
		{
			helper << value.total_bytes << "b";
		}
		return helper;
	}
}