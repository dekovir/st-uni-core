#pragma once
#include "unicore/Plugin.hpp"

namespace unicore
{
	class XMLPlugin : public Plugin
	{
		UC_OBJECT(XMLPlugin, Plugin)
	public:
		void register_module(Context& context) override;
		void unregister_module(Context& context) override;
	};
}