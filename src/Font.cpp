#include "unicore/Font.hpp"
#include "unicore/Data.hpp"

namespace unicore
{
	size_t Font::system_memory_use() const
	{
		// TODO: Add kerning dictionary size;
		return sizeof(Font);
	}

	int Font::find_kerning(uint32_t a, uint32_t b) const
	{
		if (const auto it = kerning.find(a); it != kerning.end())
		{
			if (const auto jt = it->second.find(b); jt != it->second.end())
				return jt->second;
		}

		return 0;
	}

	Shared<Texture> Font::print_char(uint32_t code, Vector2f& pos,
		VertexTexColor2& v0, VertexTexColor2& v1,
		VertexTexColor2& v2, VertexTexColor2& v3, const Color4b& color) const
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

			return tex;
		}

		return nullptr;
	}
}