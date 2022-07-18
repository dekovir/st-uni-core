#pragma once
#include "unicore/Defs.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	ifndef _WIN32_WINNT // Allow use of features specific to Windows XP or later.
#		define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target other versions of Windows.
#	endif
#include <Windows.h>

namespace unicore
{
	
}
#endif