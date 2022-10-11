#include "unicore/stb/StbTTFontFactory.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#include "unicore/BinaryData.hpp"
#include "unicore/Logger.hpp"
#include "unicore/Canvas.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Renderer.hpp"
#include "unicore/stb/StbRectPack.hpp"
#include "unicore/stb/StbTTFont.hpp"

namespace unicore
{
	class Context;

	StbTTFontFactory::StbTTFontFactory(
		Renderer& renderer, const Shared<BinaryData>& data)
		: _renderer(renderer)
		, _data(data)
	{
		_valid = stbtt_InitFont(&_font_info, _data->data_as<unsigned char>(), 0);
	}

	size_t StbTTFontFactory::get_system_memory_use() const
	{
		return sizeof(StbTTFontFactory) + _data->size();
	}

	void StbTTFontFactory::get_font_metrics(
		int* ascent, int* descent, int* line_gap) const
	{
		if (valid())
			stbtt_GetFontVMetrics(&_font_info, ascent, descent, line_gap);
	}

	void StbTTFontFactory::get_codepoint_metrics(Char32 c,
		int* advance_width, int* left_side_bearing) const
	{
		stbtt_GetCodepointHMetrics(&_font_info,
			c, advance_width, left_side_bearing);
	}

	static void bitmap_free(void* ptr)
	{
		stbtt_FreeBitmap(static_cast<unsigned char*>(ptr), nullptr);
	}

	MemoryChunk StbTTFontFactory::get_codepoint_bitmap(Char32 c,
		const Vector2f& scale, Vector2i& size, Vector2i* offset) const
	{
		const auto data = stbtt_GetCodepointBitmap(&_font_info,
			scale.x, scale.y, c, &size.x, &size.y,
			offset ? &offset->x : nullptr, offset ? &offset->y : nullptr);
		if (data)
			return MemoryChunk(data, size.x * size.y, &bitmap_free);

		return {};
	}

	Shared<TexturedFont> StbTTFontFactory::create(
		const TTFontOptions& options, Logger* logger)
	{
		if (!valid()) return nullptr;

		const auto scale = stbtt_ScaleForPixelHeight(
			&_font_info, options.height);

		StbTTFont::ConstructionParams params;

		{
			int advance;
			stbtt_GetCodepointHMetrics(&_font_info, L' ', &advance, nullptr);
			params.space_width = scale * static_cast<float>(advance);
		}

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&_font_info, &ascent, &descent, &line_gap);
		//const auto baseline = Math::round_to_int(scale * static_cast<float>(ascent));
		params.height = options.height;

		// REMOVE DUPLICATES
		Set<Char32> chars_set(options.chars.begin(), options.chars.end());
		List<Char32> chars(chars_set.begin(), chars_set.end());

		// GENERATE CHAR BITMAPS
		const size_t char_count = chars.size();
		List<unsigned char*> item_bm(char_count);
		List<Vector2i> item_size(char_count);
		List<Vector2i> item_off(char_count);

		for (unsigned i = 0; i < char_count; i++)
		{
			const auto c = chars[i];

			int w, h, xoff, yoff;
			auto bm = stbtt_GetCodepointBitmap(
				&_font_info, 0, scale, c, &w, &h, &xoff, &yoff);

			item_bm[i] = bm;
			item_size[i] = { w + 2, h + 2 };
			item_off[i] = { xoff, yoff };
		}

		// PACK CHARS
		List<Recti> item_packed(char_count);

		StbRectPack packer;
		Vector2i surface_size;

		const auto start_size = packer.calc_start_size(item_size);
		if (!packer.pack_optimize(item_size,
			item_packed, surface_size, { start_size, 16 }))
		{
			UC_LOG_ERROR(logger) << "Failed to pack";
			return nullptr;
		}

		// COPY CHARS FONT TO SURFACE
		DynamicSurface font_surface(surface_size);
		Canvas canvas(font_surface);
		canvas.fill(ColorConst4b::Clear);

		for (unsigned i = 0; i < char_count; i++)
		{
			const auto& r = item_packed[i];
			const Recti packed = { r.x + 1, r.y + 1, r.w - 2, r.h - 2 };

#if 1
			int bm_offset = 0;
			int surface_offset = packed.x + packed.y * surface_size.x;
			auto surface_data = static_cast<UInt32*>(font_surface.data());

			for (int y = 0; y < packed.h; y++, surface_offset += surface_size.x - packed.w)
				for (int x = 0; x < packed.w; x++, bm_offset++, surface_offset++)
				{
					const auto a = item_bm[i][bm_offset];
					//const Color4b color(255, 255, 255, a);
					//color.to_format(font_surface.format());
					// TODO: Use surface format
					surface_data[surface_offset] = 0x00FFFFFF + (a << 24);
				}
#else
			for (int y = 0; y < packed.h; y++)
				for (int x = 0; x < packed.w; x++)
				{
					const auto a = item_bm[i][x + y * packed.w];
					const Color4b color(255, 255, 255, a);
					canvas.draw_point({ packed.x + x, packed.y + y }, color);
				}
#endif
			stbtt_FreeBitmap(item_bm[i], nullptr);

			stbtt_bakedchar info;
			info.xoff = static_cast<float>(item_off[i].x);
			info.yoff = static_cast<float>(item_off[i].y);

			int advance;
			stbtt_GetCodepointHMetrics(&_font_info, chars[i], &advance, nullptr);
			info.xadvance = scale * static_cast<Float>(advance);
			info.x0 = static_cast<UInt16>(packed.min_x());
			info.y0 = static_cast<UInt16>(packed.min_y());
			info.x1 = static_cast<UInt16>(packed.max_x());
			info.y1 = static_cast<UInt16>(packed.max_y());

			params.infos[chars[i]] = info;
		}

		params.texture = _renderer.create_texture(font_surface);

		return std::make_shared<StbTTFont>(params);
	}
}
#endif