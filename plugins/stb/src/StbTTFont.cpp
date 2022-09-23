#include "unicore/stb/StbTTFont.hpp"
#if defined(UNICORE_USE_STB_TRUETYPE)
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace unicore
{
	size_t StbTTFont::get_system_memory_use() const
	{
		return sizeof(StbTTFont);
	}

	size_t StbTTFont::get_used_resources(Set<Shared<Resource>>& resources)
	{
		if (_texture)
		{
			resources.insert(_texture);
			return 1;
		}

		return 0;
	}

	float StbTTFont::get_height() const
	{
		return _height;
	}

	float StbTTFont::calc_width(WStringView text) const
	{
		Vector2f cur = VectorConst2f::Zero;
		for (const auto c : text)
		{
			Rectf rect, uv;
			get_char_print_info(c, cur, &rect, &uv);
		}

		return cur.x;
	}

	Shared<Texture> StbTTFont::get_char_print_info(uint32_t code,
		Vector2f& pos, Rectf* rect, Rectf* uv_rect) const
	{
		if (const auto it = _infos.find(code); it != _infos.end())
		{
			const auto b = &it->second;
			const auto& size = _texture->size();

			const float ipw = 1.0f / static_cast<float>(size.x);
			const float iph = 1.0f / static_cast<float>(size.y);
			const float round_x = Math::floor((pos.x + b->xoff) + 0.5f);
			const float round_y = Math::floor((pos.y + b->yoff) + 0.5f);

			if (rect)
			{
				*rect = Rectf::from_min_max(
					{ round_x, round_y },
					{ round_x + b->x1 - b->x0, round_y + b->y1 - b->y0 }
				);
			}

			if (uv_rect)
			{
				*uv_rect = Rectf::from_min_max(
					{ b->x0 * ipw, b->y0 * iph },
					{ b->x1 * ipw, b->y1 * iph }
				);
			}

			pos.x += b->xadvance;

			return _texture;
		}

		return nullptr;
	}

	void StbTTFont::generate(
		const Vector2f& position, WStringView text, const Color4b& color,
		Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict)
	{
		Vector2f cur = position;
		for (const auto c : text)
		{
			Rectf rect, uv;
			if (auto tex = get_char_print_info(c, cur, &rect, &uv))
			{
				VertexTexColorQuad2 quad;
				rect.y += _height;

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
				quad.v[0].col = color;

				quad.v[1].pos.set(x2, y1);
				quad.v[1].uv.set(tx2, ty1);
				quad.v[1].col = color;

				quad.v[2].pos.set(x2, y2);
				quad.v[2].uv.set(tx2, ty2);
				quad.v[2].col = color;

				quad.v[3].pos.set(x1, y2);
				quad.v[3].uv.set(tx1, ty2);
				quad.v[3].col = color;

				quad_dict[tex].push_back(quad);
			}
		}
	}
}
#endif