#include "SurfaceLoader.hpp"
#include <stb_image.h>

#include "unicore/ResourceCache.hpp"

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

	static constexpr stbi_io_callbacks s_stream_callbacks{ stbi_stream_read, stbi_stream_skip, stbi_stream_eof };
	static constexpr std::initializer_list<WStringView> s_ext = { L".png",L".tga",L".jpg",L".jpeg" };

	// SurfaceLoader //////////////////////////////////////////////////////////////
	SurfaceLoader::SurfaceLoader()
		: ResourceLoaderT(s_ext)
	{
	}

	Shared<Resource> SurfaceLoader::load(const ResourceLoaderContext& context)
	{
		int w, h, n;
		const auto data = stbi_load_from_callbacks(&s_stream_callbacks, &context.stream, &w, &h, &n, 4);
		if (!data)
		{
			UC_LOG_ERROR(context.logger) << stbi_failure_reason();
			return nullptr;
		}

		MemoryChunk chunk(w * h * 4);
		Memory::copy(chunk.data(), data, chunk.size());
		stbi_image_free(data);

		return std::make_shared<Surface>(Vector2i(w, h), chunk);
	}

	// DynamicSurfaceLoader ///////////////////////////////////////////////////////
	DynamicSurfaceLoader::DynamicSurfaceLoader()
		: ResourceLoaderT(s_ext)
	{
	}

	Shared<Resource> DynamicSurfaceLoader::load(const ResourceLoaderContext& context)
	{
		// Use cached Surface
		if (const auto cached = context.cache.find<Surface>(context.path))
		{
			auto surface = std::make_shared<DynamicSurface>(cached->size());
			Memory::copy(surface->data(), cached->data(), cached->size_bytes());
			return surface;
		}

		// Direct load to surface to prevent double memory consumption (Surface + DynamicSurface)
		int w, h, n;
		const auto data = stbi_load_from_callbacks(&s_stream_callbacks, &context.stream, &w, &h, &n, 4);
		if (!data)
		{
			UC_LOG_ERROR(context.logger) << stbi_failure_reason();
			return nullptr;
		}

		auto surface = std::make_shared<DynamicSurface>(w, h);
		Memory::copy(surface->data(), data, surface->size_bytes());
		stbi_image_free(data);

		return surface;
	}
}