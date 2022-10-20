#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class SurfacePlugin : public Plugin
	{
		UC_OBJECT(SurfacePlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}