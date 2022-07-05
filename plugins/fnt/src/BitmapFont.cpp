#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	Shared<Texture> BitmapFont::get_char_print_info(uint32_t code,
		Vector2f& pos, Rectf* rect, Rectf* uv_rect) const
	{
		if (const auto it = glyphs.find(code); it != glyphs.end())
		{
			const auto& c = it->second;
			if (c.page < pages.size())
			{
				auto page = pages[c.page];
				auto& size = page->size();

				if (uv_rect)
				{
					uv_rect->x = static_cast<float>(c.rect.x) / static_cast<float>(size.x);
					uv_rect->y = static_cast<float>(c.rect.y) / static_cast<float>(size.y);
					uv_rect->w = static_cast<float>(c.rect.w) / static_cast<float>(size.x);
					uv_rect->h = static_cast<float>(c.rect.h) / static_cast<float>(size.y);
				}

				if (rect)
				{
					rect->x = pos.x - static_cast<float>(c.xoffset);
					rect->y = pos.y + static_cast<float>(c.yoffset);
					rect->w = static_cast<float>(c.rect.w);
					rect->h = static_cast<float>(c.rect.h);
				}

				pos.x += static_cast<float>(c.xadvance);

				return page;
			}
		}

		return nullptr;
	}

	Shared<Texture> BitmapFont::print_char(uint32_t code, Vector2f& pos,
		VertexTexColor2& v0, VertexTexColor2& v1,
		VertexTexColor2& v2, VertexTexColor2& v3,
		const Color4b& color) const
	{
		if (code == 32)
		{
			pos.x += static_cast<float>(space_w);
			return nullptr;
		}

		if (const auto it = glyphs.find(code); it != glyphs.end())
		{
			const auto& c = it->second;
			if (c.page < pages.size())
			{
				auto page = pages[c.page];
				auto& size = page->size();

				const float tx1 = static_cast<float>(c.rect.x) / static_cast<float>(size.x);
				const float ty1 = static_cast<float>(c.rect.y) / static_cast<float>(size.y);
				const float tx2 = static_cast<float>(c.rect.max_x()) / static_cast<float>(size.x);
				const float ty2 = static_cast<float>(c.rect.max_y()) / static_cast<float>(size.y);

				const float x1 = pos.x - static_cast<float>(c.xoffset);
				const float y1 = pos.y + static_cast<float>(c.yoffset);
				const float x2 = x1 + static_cast<float>(c.rect.w);
				const float y2 = y1 + static_cast<float>(c.rect.h);

				v0.pos.set(x1, y1);
				v0.uv.set(tx1, ty1);
				v0.col = color;

				v1.pos.set(x2, y1);
				v1.uv.set(tx2, ty1);
				v1.col = color;

				v2.pos.set(x2, y2);
				v2.uv.set(tx2, ty2);
				v2.col = color;

				v3.pos.set(x1, y2);
				v3.uv.set(tx1, ty2);
				v3.col = color;

				pos.x += static_cast<float>(c.xadvance);

				return page;
			}
		}

		return nullptr;
	}
}