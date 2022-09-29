#include "unicore/stb/StbEasyFont.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include <stb_easy_font.h>
#include "unicore/Unicode.hpp"

namespace unicore
{
	struct Vertex
	{
		Vector2f pos;
		float z;
		uint32_t color;
	};

	// TODO: Optimize
	constexpr auto BUFFER_SIZE = 1024 * 2;
	static Vertex s_quads[BUFFER_SIZE];

	StbEasyFont::StbEasyFont()
		: _height(static_cast<float>(internal_calc_height()))
		, _scale(1)
	{
	}

	StbEasyFont::StbEasyFont(float height)
		: _height(height)
		, _scale(height / static_cast<float>(internal_calc_height()))
	{
	}

	size_t StbEasyFont::get_system_memory_use() const
	{
		return sizeof(StbEasyFont);
	}

	float StbEasyFont::calc_width(WStringView text) const
	{
		const auto str = Unicode::to_utf8(text);
		return _scale * static_cast<float>(stb_easy_font_width(const_cast<char*>(str.data())));
	}

	size_t StbEasyFont::generate(const Vector2f& position, WStringView text,
		const Color4b& color, List<VertexColorQuad2>& quad_list) const
	{
		const auto str = Unicode::to_utf8(text);
		const auto num_quads = stb_easy_font_print(
			0, 0, const_cast<char*>(str.data()),
			nullptr, s_quads, BUFFER_SIZE * sizeof(Vertex));

		for (auto i = 0; i < num_quads; i++)
		{
			VertexColorQuad2 quad;
			quad.v[0] = { position + s_quads[i * 4 + 0].pos * _scale, color };
			quad.v[1] = { position + s_quads[i * 4 + 1].pos * _scale, color };
			quad.v[2] = { position + s_quads[i * 4 + 2].pos * _scale, color };
			quad.v[3] = { position + s_quads[i * 4 + 3].pos * _scale, color };
			quad_list.emplace_back(quad);
		}

		return num_quads;
	}

	int StbEasyFont::internal_calc_height()
	{
		return stb_easy_font_height((char*)"W");
	}
}
#endif