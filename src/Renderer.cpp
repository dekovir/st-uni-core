#include "unicore/Renderer.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "Resources/STBSurfaceLoader.hpp"

namespace unicore
{
	class TextureConverter : public ResourceConverterT<Texture, Surface>
	{
	public:
		explicit TextureConverter(Renderer& render)
			: _renderer(render) {}

		Shared<Texture> convert_typed(Surface& surface, const ResourceConverterContext& context) override
		{
			return _renderer.create_texture(surface);
		}

	protected:
		Renderer& _renderer;
	};

	class DynamicTextureConverter : public ResourceConverterT<DynamicTexture, Surface>
	{
	public:
		explicit DynamicTextureConverter(Renderer& render)
			: _renderer(render) {}

		Shared<DynamicTexture> convert_typed(Surface& surface, const ResourceConverterContext& context) override
		{
			auto tex = _renderer.create_dynamic_texture(surface.size());
			_renderer.update_texture(*tex, surface);
			return tex;
		}

	protected:
		Renderer& _renderer;
	};

	Renderer::Renderer(Logger& logger)
		: _logger(logger)
	{}

	void Renderer::register_module(const ModuleContext& context)
	{
		Module::register_module(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
#if defined(UNICORE_USE_STB)
			cache->add_loader(std::make_shared<STBSurfaceLoader>());
			cache->add_loader(std::make_shared<STBDynamicSurfaceLoader>());
#endif

			cache->add_converter(std::make_shared<TextureConverter>(*this));
			cache->add_converter(std::make_shared<DynamicTextureConverter>(*this));
		}
	}
}