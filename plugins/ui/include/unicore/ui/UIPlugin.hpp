#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class UIPlugin : public Plugin
	{
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}