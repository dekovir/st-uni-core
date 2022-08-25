#include "TextureConverter.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	TextureConverter::TextureConverter(Renderer& render)
		: _renderer(render)
	{}

	Shared<Texture> TextureConverter::convert_typed(
		Surface& surface, const ResourceConverterContext& context)
	{
		return _renderer.create_texture(surface);
	}

	DynamicTextureConverter::DynamicTextureConverter(Renderer& render)
		: _renderer(render)
	{}

	Shared<DynamicTexture> DynamicTextureConverter::convert_typed(Surface& surface,
		const ResourceConverterContext& context)
	{
		auto tex = _renderer.create_dynamic_texture(surface.size());
		_renderer.update_texture(*tex, surface);
		return tex;
	}
}