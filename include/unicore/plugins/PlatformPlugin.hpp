#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class PlatformPlugin : public Plugin
	{
		UC_OBJECT(PlatformPlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}