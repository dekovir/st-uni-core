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
		: _height(static_cast<float>(stb_easy_font_height((char*)"W")))
	{
	}

	size_t StbEasyFont::get_system_memory_use() const
	{
		return sizeof(StbEasyFont);
	}

	Vector2f StbEasyFont::calc_size(WStringView text) const
	{
		const auto str = Unicode::to_utf8(text);
		const auto w = stb_easy_font_width(const_cast<char*>(str.data()));
		const auto h = stb_easy_font_height(const_cast<char*>(str.data()));
		return { static_cast<float>(w), static_cast<float>(h) };
	}

	size_t StbEasyFont::generate(const Vector2f& position,
		WStringView text, List<VertexColorQuad2>& quad_list) const
	{
		const auto str = Unicode::to_utf8(text);
		const auto num_quads = stb_easy_font_print(
			position.x, position.y, const_cast<char*>(str.data()),
			nullptr, s_quads, BUFFER_SIZE * sizeof(Vertex));

		constexpr auto color = ColorConst4b::White;

		for (auto i = 0; i < num_quads; i++)
		{
			VertexColorQuad2 quad;
			quad.v[0] = { s_quads[i * 4 + 0].pos, color };
			quad.v[1] = { s_quads[i * 4 + 1].pos, color };
			quad.v[2] = { s_quads[i * 4 + 2].pos, color };
			quad.v[3] = { s_quads[i * 4 + 3].pos, color };
			quad_list.emplace_back(quad);
		}

		return num_quads;
	}
}
#endif