#include "SZipUtils.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	static void* Alloc(void* p, size_t size)
	{
		return Memory::alloc(size);
	}

	static void Free(void* p, void* address)
	{
		Memory::free(address);
	}

	ISzAlloc* SZipUtils::get_alloc_main()
	{
		static ISzAlloc g_alloc{ &Alloc, &Free };
		return &g_alloc;
	}

	ISzAlloc* SZipUtils::get_alloc_temp()
	{
		static ISzAlloc g_alloc{ &Alloc, &Free };
		return &g_alloc;
	}
}