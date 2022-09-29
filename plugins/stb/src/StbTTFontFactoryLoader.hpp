#pragma once
#include "unicore/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/stb/StbTTFontFactory.hpp"

namespace unicore
{
	class Renderer;

	class StbTTFontFactoryLoader : public ResourceLoaderT<StbTTFontFactory>
	{
		UC_OBJECT(StbTTFontFactoryLoader, ResourceLoader)
	public:
		explicit StbTTFontFactoryLoader(Renderer& renderer);

		UC_NODISCARD Shared<Resource> load(const Context& options) override;

	protected:
		Renderer& _renderer;
	};
}
#endif