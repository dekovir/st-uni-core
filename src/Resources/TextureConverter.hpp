#pragma once
#include "unicore/ResourceConverter.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class TextureConverter : public ResourceConverterT<Texture, Surface>
	{
	public:
		explicit TextureConverter(Renderer& render);

		Shared<Texture> convert_typed(Surface& surface, const ResourceConverterContext& context) override;

	protected:
		Renderer& _renderer;
	};

	class DynamicTextureConverter : public ResourceConverterT<DynamicTexture, Surface>
	{
	public:
		explicit DynamicTextureConverter(Renderer& render);

		Shared<DynamicTexture> convert_typed(Surface& surface, const ResourceConverterContext& context) override;

	protected:
		Renderer& _renderer;
	};
}