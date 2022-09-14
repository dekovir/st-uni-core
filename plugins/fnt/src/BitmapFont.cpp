#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	BitmapFont::BitmapFont(
		PagesList pages, GlyphsDictionary glyphs,
		KerningDictionary kerning, uint8_t space_width)
		: TexturedFont(std::move(kerning), space_width),
		_pages(std::move(pages)),
		_glyphs(std::move(glyphs))
	{
	}

	size_t BitmapFont::get_system_memory_use() const
	{
		return sizeof(BitmapFont) + (sizeof(BitmapFontGlyph) + sizeof(uint32_t)) * _glyphs.size();
	}

	size_t BitmapFont::get_used_resources(Set<Shared<Resource>>& resources)
	{
		if (!_pages.empty())
		{
			for (const auto& texture : _pages)
				resources.insert(texture);
			return _pages.size();
		}

		return 0;
	}

	Shared<Texture> BitmapFont::get_char_print_info(uint32_t code,
		Vector2f& pos, Rectf* rect, Rectf* uv_rect) const
	{
		if (code == 32)
		{
			pos.x += static_cast<float>(_space_width);
			return nullptr;
		}

		if (const auto it = _glyphs.find(code); it != _glyphs.end())
		{
			const auto& c = it->second;
			if (c.page < _pages.size())
			{
				auto page = _pages[c.page];

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