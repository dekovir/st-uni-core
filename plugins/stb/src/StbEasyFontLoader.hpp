#pragma once
#include "unicore/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include "unicore/stb/StbEasyFont.hpp"

namespace unicore
{
	class StbEasyFontLoader : public ResourceLoaderTypeOptions<StbEasyFont, EmptyResourceOptions>
	{
		UC_OBJECT(StbEasyFontLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const EmptyResourceOptions& options) override;
	};
}
#endif