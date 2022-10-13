#pragma once
#include "unicore/app/Application.hpp"

namespace unicore
{
	extern Unique<Application> create_main_core(const CoreSettings& settings);
}

#define UNICORE_MAIN_CORE(CoreType) \
	Unique<Application> create_main_core(const CoreSettings& settings) \
	{ return make_unique<CoreType>(settings); }