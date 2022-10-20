#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include "unicore/stb/StbEasyFont.hpp"

namespace unicore
{
	class StbEasyFontLoader : public ResourceLoaderOptionsTyped<
		EmptyResourceOptions,
		ResourceLoaderTypePolicy::Multiple<StbEasyFont, GeometryFont>,
		ResourceLoaderPathPolicy::Empty>
	{
		UC_OBJECT(StbEasyFontLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const EmptyResourceOptions& options) override;
	};
}
#endif