#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class TTFontLoader : public ResourceLoaderTypeOptions<TexturedFont, TTFontOptions>
	{
	public:
		TTFontLoader();

		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const TTFontOptions& options) override;
	};
}