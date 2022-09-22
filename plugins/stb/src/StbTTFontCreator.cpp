#include "StbTTFontCreator.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	Shared<StbTTFont> StbTTFontCreator::create_from_data(const Options& context, const TTFOptions& data)
	{
		return nullptr;
		/*
		constexpr int size = 512;
		constexpr int total = size * size;
		constexpr int char_start = 32;
		constexpr int char_count = 96;
		constexpr float height = 32;

		context.cache.load<BinaryData>(context.)

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

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&font, &ascent, &descent, &line_gap);
		const auto baseline =
			Math::round_to_int(scale * static_cast<float>(ascent));
		const auto line_height = scale * (ascent - descent + line_gap);

		stbtt_bakedchar cdata[char_count];
		MemoryChunk buffer(total);

		int w, h, xoff, yoff;
		auto bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, 0, &w, &h, &xoff, &yoff);

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

		return std::make_shared<StbTTFont>(kerning, height, infos, tex);*/
	}
}
#endif