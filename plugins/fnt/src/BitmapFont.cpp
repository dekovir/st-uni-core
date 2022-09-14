#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	BitmapFont::BitmapFont(const ConstructionParams& params)
		: TexturedFont(params.kerning, params.space_width)
		, _pages(params.pages)
		, _glyphs(params.glyphs)
		, _height(static_cast<float>(params.height))
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

	float BitmapFont::get_height() const
	{
		return _height;
	}

	Vector2f BitmapFont::calc_size(StringView text) const
	{
		Vector2f cur = { 0, _height };
		Rectf r;
		for (size_t i = 0; i < text.size(); i++)
		{
			const auto c = text[i];
			get_char_print_info(c, cur, &r, nullptr);

			if (i + 1 < text.size())
				cur.x += static_cast<float>(find_kerning(c, text[i + 1]));
			else cur.x += r.w;
		}

		return cur;
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