#pragma once
#include "unicore/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/stb/StbTTFont.hpp"

namespace unicore
{
	class Renderer;

	class StbTTFontLoader : public ResourceLoaderT<StbTTFont>
	{
	public:
		explicit StbTTFontLoader(Renderer& renderer);

		UC_NODISCARD Shared<Resource> load(const Options& options) override;

	protected:
		Renderer& _renderer;
	};
}
#endif