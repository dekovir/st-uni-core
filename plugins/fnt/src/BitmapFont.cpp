#include "unicore/fnt/BitmapFont.hpp"

namespace unicore
{
	BitmapFont::BitmapFont(const ConstructionParams& params)
		: _pages(params.pages)
		, _glyphs(params.glyphs)
		, _kerning(params.kerning)
		, _height(static_cast<float>(params.height))
		, _space_width(params.space_width)
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

	Vector2f BitmapFont::calc_size(WStringView text) const
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

	int BitmapFont::find_kerning(uint32_t a, uint32_t b) const
	{
		if (const auto it = _kerning.find(a); it != _kerning.end())
		{
			if (const auto jt = it->second.find(b); jt != it->second.end())
				return jt->second;
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

	void BitmapFont::generate(const Vector2f& position, WStringView text,
		Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict)
	{
		Vector2f cur = position;
		for (size_t i = 0; i < text.size(); i++)
		{
			const auto c = text[i];
			VertexTexColorQuad2 quad;
			if (auto tex = print_char(c, cur, quad))
			{
				quad_dict[tex].push_back(quad);
				if (i + 1 < text.size())
					cur.x += static_cast<float>(find_kerning(c, text[i + 1]));
			}
		}
	}

	Shared<Texture> BitmapFont::print_char(uint32_t code,
		Vector2f& pos, VertexTexColorQuad2& quad) const
	{
		Rectf rect, uv;
		if (auto tex = get_char_print_info(code, pos, &rect, &uv))
		{
			const float x1 = rect.min_x();
			const float y1 = rect.min_y();
			const float x2 = rect.max_x();
			const float y2 = rect.max_y();

			const float tx1 = uv.min_x();
			const float ty1 = uv.min_y();
			const float tx2 = uv.max_x();
			const float ty2 = uv.max_y();

			quad.v[0].pos.set(x1, y1);
			quad.v[0].uv.set(tx1, ty1);
			quad.v[0].col = ColorConst4b::White;

			quad.v[1].pos.set(x2, y1);
			quad.v[1].uv.set(tx2, ty1);
			quad.v[1].col = ColorConst4b::White;

			quad.v[2].pos.set(x2, y2);
			quad.v[2].uv.set(tx2, ty2);
			quad.v[2].col = ColorConst4b::White;

			quad.v[3].pos.set(x1, y2);
			quad.v[3].uv.set(tx1, ty2);
			quad.v[3].col = ColorConst4b::White;

			return tex;
		}

		return nullptr;
	}
}