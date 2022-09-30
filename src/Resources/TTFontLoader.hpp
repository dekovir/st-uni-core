#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class TTFontLoader : public ResourceLoaderOptionsTyped<
		TexturedFont, TTFontOptions,
		ResourceLoaderPathPolicy::NotEmpty,
		ResourceLoaderOptionsPolicy::NullOrExact<TTFontOptions>>
	{
	public:
		UC_NODISCARD Shared<Resource> load_options(
			const Context& context, const TTFontOptions& options) override;
	};
}