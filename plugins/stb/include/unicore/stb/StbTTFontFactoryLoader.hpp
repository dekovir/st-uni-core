#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/stb/StbTTFontFactory.hpp"

namespace unicore
{
	class Renderer;

	class StbTTFontFactoryLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<StbTTFontFactory, TTFontFactory>,
		TTFontLoadPathPolicy>
	{
		UC_OBJECT(StbTTFontFactoryLoader, ResourceLoader)
	public:
		explicit StbTTFontFactoryLoader(Renderer& renderer);

		UC_NODISCARD Shared<Resource> load(const Context& context) override;

	protected:
		Renderer& _renderer;
	};
}
#endif