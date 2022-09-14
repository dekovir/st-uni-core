#include "EasyFont.hpp"
#if defined(UNICORE_USE_STB_EASY_FONT)
#include "unicore/Vector3.hpp"
#include <stb_easy_font.h>

namespace unicore
{
	struct Vertex
	{
		float x, y, z;
		uint32_t color;
	};

	// TODO: Optimize
	constexpr auto BUFFER_SIZE = 1024 * 2;
	static Vertex s_quads[BUFFER_SIZE];

	size_t EasyFont::get_system_memory_use() const
	{
		return sizeof(EasyFont);
	}

	size_t EasyFont::print_quads(const Vector2f& position, StringView text, List<VertexColor2>& quads) const
	{
		const auto num_quads = stb_easy_font_print(
			position.x, position.y, (char*)text.data(),
			nullptr, s_quads, BUFFER_SIZE * sizeof(Vertex));

		constexpr auto color = ColorConst4b::White;

		for (auto i = 0; i < num_quads; i++)
		{
			const auto& v0 = s_quads[i * 4 + 0];
			const auto& v1 = s_quads[i * 4 + 1];
			const auto& v2 = s_quads[i * 4 + 2];
			const auto& v3 = s_quads[i * 4 + 3];

			quads.emplace_back(Vector2f(v0.x, v0.y), color);
			quads.emplace_back(Vector2f(v1.x, v1.y), color);
			quads.emplace_back(Vector2f(v2.x, v2.y), color);
			quads.emplace_back(Vector2f(v3.x, v3.y), color);
		}

		return num_quads;
	}
}
#endif