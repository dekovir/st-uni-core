#include "unicore/Render2D.hpp"
#include "unicore/Data.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "SDL2/SDL2Render.hpp"
#include <stb_image.h>

namespace unicore
{
	class SurfaceLoader : public ResourceLoaderT<Surface>
	{
	public:
		UC_NODISCARD bool can_load_extension(WStringView ext) const override
		{
			return
				ext == L".png" || ext == L".tga" ||
				ext == L".jpg" || ext == L".jpeg";
		}

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override
		{
			const auto data = context.cache.load<BinaryData>(context.path);
			if (!data)
			{
				UC_LOG_ERROR(context.logger) << "Failed to load BinaryData from " << context.path;
				return nullptr;
			}

			const auto buffer = (stbi_uc*)data->data();
			const auto size = static_cast<int>(data->size());

			int w, h, n;
			auto rgba = stbi_load_from_memory(buffer, size, &w, &h, &n, 4);
			if (!rgba)
			{
				UC_LOG_ERROR(context.logger) << stbi_failure_reason();
				return nullptr;
			}

			auto surface = std::make_shared<BitmapSurface>(w, h);
			Memory::copy(surface->data(), rgba, surface->data_size());

			stbi_image_free(rgba);
			return surface;
		}
	};

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
		Render::register_module(context);

		static Render2DTextureLoader texture_loader(*this);
		static SurfaceLoader surface_loader;

		context.add_loader(texture_loader);
		context.add_loader(surface_loader);
	}

	void Render2D::unregister_module(Context& context)
	{
		Render::unregister_module(context);
	}

	Unique<Render2D> Render2D::create(Logger& logger)
	{
#if defined(UNICORE_USE_SDL2)
		SDL2RenderSettings settings;
		settings.size = Vector2iConst::Zero;
		//settings.size = Vector2i(800, 600);
		settings.resizeable = false;
		settings.borderless = false;
		settings.fullscreen = false;
		return make_unique<SDL2Render>(logger, settings);
#else
		UC_STATIC_ASSERT_ALWAYS("Unknown platform");
#endif
	}
}