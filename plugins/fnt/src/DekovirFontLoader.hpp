#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	struct DekovirFontLoadPolicy : ResourceLoaderPolicy::ExtensionPolicy
	{
		DekovirFontLoadPolicy()
			: ExtensionPolicy({ L".fnt" })
		{
		}
	};

	class DekovirFontLoader : public ResourceLoaderType<BitmapFont, DekovirFontLoadPolicy>
	{
	public:
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}