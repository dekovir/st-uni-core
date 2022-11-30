#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore::remoteui
{
	class RemoteUIPlugin : public Plugin
	{
	public:
		void register_plugin(const ModuleContext& context) override;
	};
}