#include "unicore/loaders/TextureLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	// TextureLoader //////////////////////////////////////////////////////////////
	TextureLoader::TextureLoader(Renderer& render)
		: _renderer(render)
	{}

	Shared<Resource> TextureLoader::load(const Context& context)
	{
		const auto surface = context.cache.load<Surface>(context.path);
		// TODO: Insert log message on failed?
		return surface ? _renderer.create_texture(*surface) : nullptr;
	}

	// DynamicTextureLoader ///////////////////////////////////////////////////////
	DynamicTextureLoader::DynamicTextureLoader(Renderer& render)
		: _renderer(render)
	{}

	Shared<Resource> DynamicTextureLoader::load(const Context& context)
	{
		if (const auto surface = context.cache.load<Surface>(context.path))
		{
			auto tex = _renderer.create_dynamic_texture(surface->size());
			_renderer.update_texture(*tex, *surface);
			return tex;
		}

		// TODO: Insert log message on failed?
		return nullptr;
	}
}