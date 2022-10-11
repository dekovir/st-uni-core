#pragma once
#include "unicore/app/Core.hpp"

namespace unicore
{
	extern Unique<Core> create_main_core(const CoreSettings& settings);
}

#define UNICORE_MAIN_CORE(CoreType) \
	Unique<Core> create_main_core(const CoreSettings& settings) \
	{ return make_unique<CoreType>(settings); }