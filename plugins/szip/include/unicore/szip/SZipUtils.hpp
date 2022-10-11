#pragma once
#include "unicore/Defs.hpp"
#include <7z.h>

namespace unicore
{
	namespace SZipUtils
	{
		extern ISzAlloc* get_alloc_main();
		extern ISzAlloc* get_alloc_temp();
	};
}