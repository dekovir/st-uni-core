#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class WasmPlugin : public Plugin
	{
	public:
		WasmPlugin() = default;

		void register_module(Context& context) override;
	};
}