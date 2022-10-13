#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/renderer/Font.hpp"

namespace unicore
{
	class TTFontLoader : public ResourceLoaderOptionsTyped<
		TTFontOptions,
		ResourceLoaderTypePolicy::Multiple<TexturedFont, Font>,
		ResourceLoaderPathPolicy::NotEmpty,
		ResourceLoaderOptionsPolicy::NullOrExact<TTFontOptions>>
	{
		UC_OBJECT(TTFontLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const TTFontOptions& options) override;
	};
}