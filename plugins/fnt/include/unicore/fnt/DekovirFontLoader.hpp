#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	struct DekovirFontLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		DekovirFontLoadPolicy()
			: Extension({ ".fnt" })
		{
		}
	};

	class DekovirFontLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<BitmapFont, Font>,
		DekovirFontLoadPolicy>
	{
		UC_OBJECT(DekovirFontLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}