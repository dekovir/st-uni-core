#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class ResourcePlugin : public Plugin
	{
		UC_OBJECT(ResourcePlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}