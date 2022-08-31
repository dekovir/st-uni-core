#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class PlatformPlugin : public Plugin
	{
		UC_OBJECT(PlatformPlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}