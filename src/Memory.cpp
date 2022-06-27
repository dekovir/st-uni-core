#include "unicore/Memory.hpp"

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
}