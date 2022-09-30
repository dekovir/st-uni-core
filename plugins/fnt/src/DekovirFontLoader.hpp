#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	class DekovirFontLoader : public ResourceLoaderType<BitmapFont>
	{
	public:
		DekovirFontLoader();
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}