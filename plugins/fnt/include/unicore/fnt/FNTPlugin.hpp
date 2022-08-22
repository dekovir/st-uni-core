#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class FNTPlugin : public Plugin
	{
		UC_OBJECT(FNTPlugin, Plugin)
	public:
		FNTPlugin() = default;

		void register_module(const ModuleContext& context) override;
	};
}