#include "unicore/Memory.hpp"

namespace unicore
{
	void* Memory::alloc(size_t size)
	{
		return ::alloca(size);
	}

	void Memory::free(void* ptr)
	{
		::free(ptr);
	}
}
