#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class StbPlugin : public Plugin
	{
		UC_OBJECT(StbPlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}