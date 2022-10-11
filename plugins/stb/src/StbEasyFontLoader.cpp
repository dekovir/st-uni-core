#include "unicore/stb/StbEasyFontLoader.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
namespace unicore
{
	Shared<Resource> StbEasyFontLoader::load_options(
		const Context& context, const EmptyResourceOptions& options)
	{
		return std::make_shared<StbEasyFont>();
	}
}
#endif