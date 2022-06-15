#pragma once
#include "unicore/Core.hpp"

namespace unicore
{
	extern Shared<Core> create_main_core(Platform& platform);
}

#define UNICORE_MAIN_CORE(CoreType) \
	Shared<Core> create_main_core(Platform& platform) \
	{ return make_shared<CoreType>(platform); }