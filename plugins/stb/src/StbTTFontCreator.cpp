#include "StbTTFontCreator.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/Logger.hpp"
#include "unicore/BinaryData.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/stb/StbRectPack.hpp"

namespace unicore
{
	Shared<StbTTFont> StbTTFontCreator::create_from_data(const Options& context, const TTFOptions& data)
	{
		const auto chunk = context.cache.load<BinaryData>(data.path, ResourceCacheFlag::IgnoreExtension);
		if (!chunk)
		{
			UC_LOG_ERROR(context.logger) << "Failed to create font";
			return nullptr;
		}

		stbtt_fontinfo font;
		if (!stbtt_InitFont(&font, static_cast<const unsigned char*>(chunk->data()), 0))
		{
			UC_LOG_ERROR(context.logger) << "Failed to init font";
			return nullptr;
		}

		StbTTFont::ConstructionParams params;
		const auto scale = stbtt_ScaleForPixelHeight(&font, static_cast<float>(data.height));

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&font, &ascent, &descent, &line_gap);
		//const auto baseline = Math::round_to_int(scale * static_cast<float>(ascent));
		params.height = scale * static_cast<float>(ascent - descent + line_gap);

		// GENERATE CHAR BITMAPS
		const size_t char_count = data.chars.size();
		List<unsigned char*> item_bm(char_count);
		List<Vector2i> item_size(char_count);
		List<Vector2i> item_off(char_count);
		for (unsigned i = 0; i < char_count; i++)
		{
			const auto  c = data.chars[i];

			item_bm[i] = stbtt_GetCodepointBitmap(&font, 0, scale, c,
				&item_size[i].x, &item_size[i].y, &item_off[i].x, &item_off[i].y);
		}

		// PACK CHARS
		List<Recti> item_rect(char_count);

		StbRectPack packer;
		Vector2i surface_size;

		const auto start_size = packer.calc_start_size(item_size);

		if (!packer.pack_optimize(item_size, item_rect, surface_size, { start_size, 16 }))
		{
			UC_LOG_ERROR(context.logger) << "Failed to pack";
			return nullptr;
		}

		// COPY TO SURFACE
		DynamicSurface surface(surface_size);
		for (unsigned i = 0; i < char_count; i++)
		{
			const auto rect = item_rect[i];
			DynamicSurface char_surface(rect.w, rect.h);
			Canvas canvas(char_surface);

			canvas.fill(ColorConst4b::Clear);

			for (auto y = 0; y < rect.w; y++)
				for (auto x = 0; x < rect.h; x++)
				{
					const auto a = item_bm[i][x + y * rect.w];
					char_surface.set(x, y, Color4b(255, 255, 255, a));
				}
			stbtt_FreeBitmap(item_bm[i], nullptr);

			canvas.copy_to(surface, rect.position());

			stbtt_bakedchar info;
			info.xoff = static_cast<float>(item_off[i].x);
			info.yoff = static_cast<float>(item_off[i].y);
			info.xadvance = rect.w;
			info.x0 = rect.min_x();
			info.y0 = rect.min_y();
			info.x1 = rect.max_x();
			info.y1 = rect.max_y();

			params.infos[data.chars[i]] = info;
		}

		params.texture = _renderer.create_texture(surface);

		return std::make_shared<StbTTFont>(params);
	}
}
#endif