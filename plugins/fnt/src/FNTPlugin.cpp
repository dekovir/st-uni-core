#include "unicore/fnt/FNTPlugin.hpp"
#include "BitmapFontLoader.hpp"

namespace unicore
{
	static BitmapFontLoader fnt_loader;

	void FNTPlugin::register_module(Context& context)
	{
		Plugin::register_module(context);
		context.add_loader(fnt_loader);
	}

	void FNTPlugin::unregister_module(Context& context)
	{
		Plugin::unregister_module(context);
		context.remove_loader(fnt_loader);
	}
}