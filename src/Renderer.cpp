#include "unicore/Renderer.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "SurfaceLoader.hpp"

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

	Renderer::Renderer(Logger& logger)
		: _logger(logger)
	{}

	void Renderer::register_module(const ModuleContext& context)
	{
		Module::register_module(context);

		if (const auto cache = context.modules.find<ResourceCache>())
		{
			cache->add_loader(std::make_shared<SurfaceLoader>());
			cache->add_converter(std::make_shared<TextureConverter>(*this));
		}
	}
}