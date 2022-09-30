#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class TTFontLoader : public ResourceLoaderTypeOptions<TexturedFont, TTFontOptions, ResourceLoaderPolicy::NotEmptyPath>
	{
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const TTFontOptions& options) override;
	};
}