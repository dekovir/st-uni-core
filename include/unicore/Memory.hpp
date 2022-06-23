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
}
