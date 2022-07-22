#pragma once
#include "unicore/Context.hpp"

namespace unicore
{
	class Module : public Object
	{
		UC_OBJECT(Module, Object)
	public:
		virtual void register_module(UC_UNUSED Context& context) {}
		virtual void unregister_module(UC_UNUSED Context& context) {}
	};
}