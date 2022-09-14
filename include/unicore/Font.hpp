#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class Font : public Resource
	{
		UC_OBJECT(Font, Resource)
	public:
		UC_NODISCARD virtual float get_height() const = 0;
		UC_NODISCARD virtual Vector2f calc_size(StringView text) const = 0;
	};

	class GeometryFont : public Font
	{
		UC_OBJECT(GeometryFont, Font)
	public:
		virtual size_t print_quads(const Vector2f& position, StringView text, List<VertexColor2>& quads) const = 0;
	};

	class TexturedFont : public Font
	{
		UC_OBJECT(TexturedFont, Font)
	public:
		using KerningDictionary = Dictionary<uint32_t, Dictionary<uint32_t, int>>;

		UC_NODISCARD int find_kerning(uint32_t a, uint32_t b) const;

		virtual Shared<Texture> get_char_print_info(uint32_t code,
			Vector2f& pos, Rectf* rect, Rectf* uv_rect) const = 0;

		virtual Shared<Texture> print_char(uint32_t code, Vector2f& pos,
			VertexTexColor2& v0, VertexTexColor2& v1,
			VertexTexColor2& v2, VertexTexColor2& v3,
			const Color4b& color = ColorConst4b::White) const;

	protected:
		const KerningDictionary _kerning;
		const uint8_t _space_width = 0;

		explicit TexturedFont(KerningDictionary kerning, uint8_t space_width = 0);
	};
}