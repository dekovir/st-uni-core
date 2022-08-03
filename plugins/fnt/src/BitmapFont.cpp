#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	size_t BitmapFont::system_memory_use() const
	{
		return Font::system_memory_use() +
			(sizeof(BitmapFontGlyph) + sizeof(uint32_t)) * glyphs.size();
	}

	Shared<Texture> BitmapFont::get_char_print_info(uint32_t code,
		Vector2f& pos, Rectf* rect, Rectf* uv_rect) const
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

				if (uv_rect)
				{
					auto& size = page->size();
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
}