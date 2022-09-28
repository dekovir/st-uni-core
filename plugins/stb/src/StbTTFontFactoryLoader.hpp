#pragma once
#include "unicore/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/stb/StbTTFontFactory.hpp"

namespace unicore
{
	class StbTTFontFactoryLoader : public ResourceLoaderT<StbTTFontFactory>
	{
		UC_OBJECT(StbTTFontFactoryLoader, ResourceLoader)
	public:
		StbTTFontFactoryLoader();
		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}
#endif