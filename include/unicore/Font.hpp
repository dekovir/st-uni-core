#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Vertex2.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class Font : public Resource
	{
	public:
		Dictionary<uint32_t, Dictionary<uint32_t, int>> kerning;

		UC_NODISCARD int find_kerning(uint32_t a, uint32_t b) const;
	};

	struct BitmapFontGlyph
	{
		uint8_t page;
		Recti rect;
		int xoffset;
		int yoffset;
		int xadvance;
	};

	class BitmapFont : public Font
	{
	public:
		UC_NODISCARD size_t system_memory_use() const override { return sizeof(BitmapFont); }

		List<Shared<Texture>> pages;
		Dictionary<uint32_t, BitmapFontGlyph> glyphs;

		Shared<Texture> get_char_print_info(uint32_t code, Vector2f& pos, Rectf* rect, Rectf* uv_rect) const;

		Shared<Texture> print_char(uint32_t code, Vector2f& pos,
			Vertex2& v0, Vertex2& v1, Vertex2& v2, Vertex2& v3,
			const Color4b& color = ColorConst4b::White) const;
	};

	class BitmapFontLoader : public ResourceLoaderT<BitmapFont>
	{
	public:
		UC_NODISCARD bool can_load_extension(WStringView ext) const override;
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}