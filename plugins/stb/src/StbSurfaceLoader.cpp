#include "StbSurfaceLoader.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/ResourceCache.hpp"
#define STB_IMAGE_IMPLEMENTATION
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

	// StbSurfaceLoader ///////////////////////////////////////////////////////////
	Shared<Resource> StbSurfaceLoader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.load<ReadFile>(context.path);
		if (!file) return nullptr;

		int w, h, n;
		const auto data = stbi_load_from_callbacks(
			&s_stream_callbacks, file.get(), &w, &h, &n, 4);
		if (!data)
		{
			UC_LOG_ERROR(context.logger) << stbi_failure_reason();
			return nullptr;
		}

		return std::make_shared<Surface>(
			Vector2i(w, h), MemoryChunk(data, w * h * 4, &stbi_image_free));
	}

	// StbDynamicSurfaceLoader ////////////////////////////////////////////////////
	Shared<Resource> StbDynamicSurfaceLoader::load(const Context& context)
	{
		// Use cached Surface
		if (const auto cached = context.cache.find<Surface>(context.path))
		{
			auto surface = std::make_shared<DynamicSurface>(cached->size());
			Memory::copy(surface->data(), cached->data(), cached->size_bytes());
			return surface;
		}

		// TODO: Log open_read failed
		const auto file = context.cache.load<ReadFile>(context.path);
		if (!file) return nullptr;

		// Direct load to surface to prevent double memory consumption (Surface + DynamicSurface)
		int w, h, n;
		const auto data = stbi_load_from_callbacks(&s_stream_callbacks, file.get(), &w, &h, &n, 4);
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
#endif