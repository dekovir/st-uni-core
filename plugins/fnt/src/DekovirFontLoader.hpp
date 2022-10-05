#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	struct DekovirFontLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		DekovirFontLoadPolicy()
			: Extension({ L".fnt" })
		{
		}
	};

	class DekovirFontLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<BitmapFont, Font>,
		DekovirFontLoadPolicy>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}