#include "unicore/Render2D.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "SDL2/SDL2Render.hpp"

namespace unicore
{
	class Render2DTextureLoader : public ResourceLoaderT<Texture>
	{
	public:
		explicit Render2DTextureLoader(Render2D& render) : _render(render) {}

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override
		{
			if (const auto surface = context.cache.load<Surface>(context.path))
				return _render.create_texture(*surface);

			UC_LOG_WARNING(context.logger) << "Failed to load surface";
			return nullptr;
		}

	protected:
		Render2D& _render;
	};

	Render2D::Render2D(Logger& logger)
		: Render(logger)
	{
	}

	void Render2D::register_module(Context& context)
	{
		static Render2DTextureLoader texture_loader(*this);

		Render::register_module(context);
		context.add_loader(texture_loader);
	}

	void Render2D::unregister_module(Context& context)
	{
		Render::unregister_module(context);
	}

	Shared<Render2D> Render2D::create(Logger& logger)
	{
#if defined(UNICORE_USE_SDL2)
		SDL2RenderSettings settings;
		settings.size = unicore::Vector2i::Zero;
		//settings.size = unicore::Vector2i(800, 600);
		settings.resizeable = false;
		settings.borderless = false;
		settings.fullscreen = false;
		return make_shared<SDL2Render>(logger, settings);
#else
		UC_STATIC_ASSERT_ALWAYS("Unknown platform");
#endif
	}
}