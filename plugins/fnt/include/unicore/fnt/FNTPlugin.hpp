#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class FNTPlugin : public Plugin
	{
		UC_OBJECT(FNTPlugin, Plugin)
	public:
		FNTPlugin() = default;

		void register_plugin(const ModuleContext& context) override;
	};
}