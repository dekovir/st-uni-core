#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class Font : public Resource
	{
	public:
		Dictionary<uint32_t, Dictionary<uint32_t, int>> kerning;
		uint8_t space_w = 0;

		UC_NODISCARD size_t system_memory_use() const override;

		UC_NODISCARD int find_kerning(uint32_t a, uint32_t b) const;

		virtual Shared<Texture> get_char_print_info(uint32_t code,
			Vector2f& pos, Rectf* rect, Rectf* uv_rect) const = 0;

		virtual Shared<Texture> print_char(uint32_t code, Vector2f& pos,
			VertexTexColor2& v0, VertexTexColor2& v1,
			VertexTexColor2& v2, VertexTexColor2& v3,
			const Color4b& color = ColorConst4b::White) const = 0;
	};
}