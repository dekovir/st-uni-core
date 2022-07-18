#pragma once
#include "unicore/app/Core.hpp"

namespace unicore
{
	extern Unique<Core> create_main_core(const Core::Settings& settings);
}

#define UNICORE_MAIN_CORE(CoreType) \
	Unique<Core> create_main_core(const Core::Settings& settings) \
	{ return make_unique<CoreType>(settings); }