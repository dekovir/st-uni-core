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
		return 10;
	}

	Vector2f StbTTFont::calc_size(StringView text) const
	{
		return VectorConst2f::Zero;
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
			const float round_x = STBTT_ifloor((pos.x + b->xoff) + 0.5f);
			const float round_y = STBTT_ifloor((pos.y + b->yoff) + 0.5f);

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
}
#endif