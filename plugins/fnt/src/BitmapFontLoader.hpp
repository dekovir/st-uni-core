#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	class BitmapFontLoader : public ResourceLoaderT<BitmapFont>
	{
	public:
		BitmapFontLoader();
		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};
}