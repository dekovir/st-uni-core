#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	class DekovirFontLoader : public ResourceLoaderT<BitmapFont>
	{
	public:
		DekovirFontLoader();
		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};
}