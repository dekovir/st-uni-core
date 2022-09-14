#include "STBSurfaceLoader.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/ResourceCache.hpp"
#include <stb_image.h>

namespace unicore
{
	// fill 'data' with 'size' bytes.  return number of bytes actually read
	static int stbi_stream_read(void* user, char* data, int size)
	{
		const auto stream = static_cast<ReadFile*>(user);
		size_t read;
		stream->read(data, size, &read);
		return static_cast<int>(read);
	}

	// skip the next 'n' bytes, or 'unget' the last -n bytes if negative
	static void stbi_stream_skip(void* user, int n)
	{
		const auto stream = static_cast<ReadFile*>(user);
		stream->seek(n, SeekMethod::Current);
	}

	// returns nonzero if we are at end of file/data
	static int stbi_stream_eof(void* user)
	{
		const auto stream = static_cast<ReadFile*>(user);
		return stream->eof() ? 1 : 0;
	}

	static constexpr stbi_io_callbacks s_stream_callbacks{ stbi_stream_read, stbi_stream_skip, stbi_stream_eof };
	static constexpr std::initializer_list<WStringView> s_ext = { L".png",L".tga",L".jpg",L".jpeg" };

	// STBSurfaceLoader ///////////////////////////////////////////////////////////
	STBSurfaceLoader::STBSurfaceLoader()
		: ResourceLoaderT(s_ext)
	{
	}

	Shared<Resource> STBSurfaceLoader::load(const Options& options)
	{
		int w, h, n;
		const auto data = stbi_load_from_callbacks(
			&s_stream_callbacks, &options.file, &w, &h, &n, 4);
		if (!data)
		{
			UC_LOG_ERROR(options.logger) << stbi_failure_reason();
			return nullptr;
		}

		return std::make_shared<Surface>(
			Vector2i(w, h), MemoryChunk(data, w * h * 4, &stbi_image_free));
	}

	// STBDynamicSurfaceLoader ////////////////////////////////////////////////////
	STBDynamicSurfaceLoader::STBDynamicSurfaceLoader()
		: ResourceLoaderT(s_ext)
	{
	}

	Shared<Resource> STBDynamicSurfaceLoader::load(const Options& options)
	{
		// Use cached Surface
		if (const auto cached = options.cache.find<Surface>(options.path))
		{
			auto surface = std::make_shared<DynamicSurface>(cached->size());
			Memory::copy(surface->data(), cached->data(), cached->size_bytes());
			return surface;
		}

		// Direct load to surface to prevent double memory consumption (Surface + DynamicSurface)
		int w, h, n;
		const auto data = stbi_load_from_callbacks(&s_stream_callbacks, &options.file, &w, &h, &n, 4);
		if (!data)
		{
			UC_LOG_ERROR(options.logger) << stbi_failure_reason();
			return nullptr;
		}

		auto surface = std::make_shared<DynamicSurface>(w, h);
		Memory::copy(surface->data(), data, surface->size_bytes());
		stbi_image_free(data);

		return surface;
	}
}
#endif