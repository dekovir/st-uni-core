#pragma once
#include "unicore/platform/Plugin.hpp"

namespace unicore
{
	class ImGuiPlugin : public Plugin
	{
		UC_OBJECT(ImGuiPlugin, Plugin)
	public:
		ImGuiPlugin() = default;
	};
}