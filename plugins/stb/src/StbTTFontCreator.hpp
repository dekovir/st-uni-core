#pragma once
#include "unicore/ResourceCreator.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/stb/StbTTFont.hpp"

namespace unicore
{
	class StbTTFontCreator : public ResourceCreatorData<StbTTFont, TTFOptions>
	{
		UC_OBJECT(StbTTFontCreator, ResourceCreator)
	public:
		explicit StbTTFontCreator(Renderer& renderer)
			: _renderer(renderer)
		{
		}

	protected:
		Renderer& _renderer;
		Shared<StbTTFont> create_from_data(const Options& context, const TTFOptions& data) override;
	};
}
#endif