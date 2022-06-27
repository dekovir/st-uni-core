#pragma once
#include "unicore/Core.hpp"

namespace unicore
{
	extern Shared<Core> create_main_core(const CoreSettings& settings);
}

#define UNICORE_MAIN_CORE(CoreType) \
	Shared<Core> create_main_core(const CoreSettings& settings) \
	{ return make_shared<CoreType>(settings); }