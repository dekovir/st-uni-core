#include "unicore/Renderer.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include <stb_image.h>

namespace unicore
{
	// fill 'data' with 'size' bytes.  return number of bytes actually read
	static int stbi_stream_read(void* user, char* data, int size)
	{
		const auto stream = static_cast<ReadStream*>(user);
		size_t read;
		stream->read(data, size, &read);
		return static_cast<int>(read);
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
		SurfaceLoader()
			: ResourceLoaderT({ L".png",L".tga",L".jpg",L".jpeg" })
		{
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

			auto surface = std::make_shared<Surface>(w, h);
			Memory::copy(surface->data(), data, surface->data_size());

			stbi_image_free(data);
			return surface;
		}
	};

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