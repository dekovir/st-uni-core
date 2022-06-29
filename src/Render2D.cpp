#include "unicore/Render2D.hpp"
#include "unicore/Data.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "SDL2/SDL2Render.hpp"
#include <stb_image.h>

namespace unicore
{
	// fill 'data' with 'size' bytes.  return number of bytes actually read
	static int stbi_stream_read(void* user, char* data, int size)
	{
		const auto stream = static_cast<ReadStream*>(user);
		size_t read;
		stream->read(data, size, &read);
		return read;
	}

	// skip the next 'n' bytes, or 'unget' the last -n bytes if negative
	static void stbi_stream_skip(void* user, int n)
	{
		const auto stream = static_cast<ReadStream*>(user);
		stream->seek(n, SeekMethod::Current);
	}

	// returns nonzero if we are at end of file/data
	static int stbi_stream_eof(void* user)
	{
		const auto stream = static_cast<ReadStream*>(user);
		return stream->eof() ? 1 : 0;
	}

	static const stbi_io_callbacks stbi_stream_callbacks{ stbi_stream_read, stbi_stream_skip, stbi_stream_eof };

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
			int w, h, n;
			const auto data = stbi_load_from_callbacks(&stbi_stream_callbacks, &context.stream, &w, &h, &n, 4);
			if (!data)
			{
				UC_LOG_ERROR(context.logger) << stbi_failure_reason();
				return nullptr;
			}

			auto surface = std::make_shared<BitmapSurface>(w, h);
			Memory::copy(surface->data(), data, surface->data_size());

			stbi_image_free(data);
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
		settings.size = VectorConst2i::Zero;
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