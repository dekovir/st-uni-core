#include "StbTTFontLoader.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/Memory.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	StbTTFontLoader::StbTTFontLoader(Renderer& renderer)
		: ResourceLoaderT({ L".ttf" })
		, _renderer(renderer)
	{
	}

	Shared<Resource> StbTTFontLoader::load(const Options& options)
	{
		constexpr int size = 512;
		constexpr int total = size * size;
		constexpr int char_start = 32;
		constexpr int char_count = 96;
		constexpr float height = 15;

		MemoryChunk chunk(options.file.size());

		options.file.seek(0);
		if (!options.file.read(chunk))
		{
			UC_LOG_ERROR(options.logger) << "Failed to read";
			return nullptr;
		}

		stbtt_fontinfo font;
		if (!stbtt_InitFont(&font, static_cast<const unsigned char*>(chunk.data()), 0))
		{
			UC_LOG_ERROR(options.logger) << "Failed to init font";
			return nullptr;
		}

		const auto scale = stbtt_ScaleForPixelHeight(&font, height);

		stbtt_bakedchar cdata[char_count];
		MemoryChunk buffer(total);

		const auto result = stbtt_BakeFontBitmap(
			(const unsigned char*)chunk.data(), 0,
			height, (unsigned char*)buffer.data(), size, size,
			char_start, char_count, cdata);

		DynamicSurface surface(size, size);
		for (auto i = 0; i < total; i++)
		{
			const auto a = static_cast<const Byte*>(buffer.data())[i];
			static_cast<UInt32*>(surface.data())[i] = Color4b(255, 255, 255, a).to_format(surface.format());
		}

		StbTTFont::ConstructionParams params;
		{
			int advance;
			stbtt_GetCodepointHMetrics(&font, L' ', &advance, nullptr);
			params.space_width = scale * static_cast<float>(advance);
		}

		params.height = height;
		params.texture = _renderer.create_texture(surface);

		for (auto i = 0; i < char_count; i++)
			params.infos.emplace(i + char_start, cdata[i]);

		return std::make_shared<StbTTFont>(params);
	}
}
#endif