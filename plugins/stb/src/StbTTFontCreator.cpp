#include "StbTTFontCreator.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/Logger.hpp"
#include "unicore/BinaryData.hpp"
#include "unicore/Canvas.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/stb/StbRectPack.hpp"

namespace unicore
{
	Shared<StbTTFont> StbTTFontCreator::create_from_data(const Options& context, const TTFOptions& data)
	{
		if (data.height <= 0)
		{
			UC_LOG_ERROR(context.logger) << "Invalid height " << data.height;
			return nullptr;
		}

		const auto chunk = context.cache.load<BinaryData>(data.path, ResourceCacheFlag::IgnoreExtension);
		if (!chunk)
		{
			UC_LOG_ERROR(context.logger) << "Load BinaryData failed";
			return nullptr;
		}

		stbtt_fontinfo font;
		if (!stbtt_InitFont(&font, static_cast<const unsigned char*>(chunk->data()), 0))
		{
			UC_LOG_ERROR(context.logger) << "Failed to init font";
			return nullptr;
		}

		const auto scale = stbtt_ScaleForPixelHeight(&font, static_cast<float>(data.height));

		StbTTFont::ConstructionParams params;

		{
			int advance;
			stbtt_GetCodepointHMetrics(&font, L' ', &advance, nullptr);
			params.space_width = scale * static_cast<float>(advance);
		}

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&font, &ascent, &descent, &line_gap);
		//const auto baseline = Math::round_to_int(scale * static_cast<float>(ascent));
		params.height = data.height; //scale * static_cast<float>(ascent - descent + line_gap);

		// GENERATE CHAR BITMAPS
		const size_t char_count = data.chars.size();
		List<unsigned char*> item_bm(char_count);
		List<Vector2i> item_size(char_count);
		List<Vector2i> item_off(char_count);

		for (unsigned i = 0; i < char_count; i++)
		{
			const auto c = data.chars[i];

			int w, h, xoff, yoff;
			auto bm = stbtt_GetCodepointBitmap(
				&font, 0, scale, c, &w, &h, &xoff, &yoff);

			item_bm[i] = bm;
			item_size[i] = { w, h };
			item_off[i] = { xoff, yoff };
		}

		// PACK CHARS
		List<Recti> item_packed(char_count);

		StbRectPack packer;
		Vector2i surface_size;

		const auto start_size = packer.calc_start_size(item_size);
		if (!packer.pack_optimize(item_size, item_packed, surface_size, { start_size, 16 }))
		{
			UC_LOG_ERROR(context.logger) << "Failed to pack";
			return nullptr;
		}

		// COPY CHARS FONT TO SURFACE
		DynamicSurface font_surface(surface_size);
		Canvas canvas(font_surface);
		canvas.fill(ColorConst4b::Clear);

		for (unsigned i = 0; i < char_count; i++)
		{
			const auto packed = item_packed[i];

			// TODO: Optimize
			for (int y = 0; y < packed.h; y++)
				for (int x = 0; x < packed.w; x++)
				{
					const auto a = item_bm[i][x + y * packed.w];
					const Color4b color(255, 255, 255, a);
					canvas.draw_point({ packed.x + x, packed.y + y }, color);
				}
			stbtt_FreeBitmap(item_bm[i], nullptr);

			stbtt_bakedchar info;
			info.xoff = static_cast<float>(item_off[i].x);
			info.yoff = static_cast<float>(item_off[i].y);

			int advance;
			stbtt_GetCodepointHMetrics(&font, data.chars[i], &advance, nullptr);
			info.xadvance = advance * scale;
			info.x0 = packed.min_x();
			info.y0 = packed.min_y();
			info.x1 = packed.max_x();
			info.y1 = packed.max_y();

			params.infos[data.chars[i]] = info;
		}

		params.texture = _renderer.create_texture(font_surface);

		return std::make_shared<StbTTFont>(params);
	}
}
#endif