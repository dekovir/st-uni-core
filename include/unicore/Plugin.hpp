#pragma once
#include "unicore/Module.hpp"

namespace unicore
{
	class Plugin : public Object
	{
		UC_OBJECT(Plugin, Object)
	public:
		virtual void register_plugin(UC_UNUSED const ModuleContext& context) {}
		virtual void unregister_plugin(UC_UNUSED const ModuleContext& context) {}
	};
}