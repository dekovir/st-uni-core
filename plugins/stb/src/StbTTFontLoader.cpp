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
		constexpr float height = 64;

		stbtt_bakedchar cdata[char_count];

		MemoryChunk chunk(options.file.size());

		options.file.seek(0);
		options.file.read(chunk);

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

		auto tex = _renderer.create_texture(surface);

		StbTTFont::KerningDictionary kerning;
		StbTTFont::CharInfo infos;

		for (auto i = 0; i < char_count; i++)
			infos.emplace(i + char_start, cdata[i]);

		return std::make_shared<StbTTFont>(kerning, height, infos, tex);
	}
}
#endif