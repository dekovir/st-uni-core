#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class RendererPlugin : public Plugin
	{
		UC_OBJECT(RendererPlugin, Plugin)
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}