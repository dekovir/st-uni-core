#pragma once
#include "unicore/Font.hpp"

namespace unicore
{
	struct BitmapFontGlyph
	{
		uint8_t page;
		Recti rect;
		int xoffset;
		int yoffset;
		int xadvance;
	};

	class BitmapFont : public TexturedFont
	{
		UC_OBJECT(BitmapFont, TexturedFont)
	public:
		using PagesList = List<Shared<Texture>>;
		using GlyphsDictionary = Dictionary<uint32_t, BitmapFontGlyph>;

		struct ConstructionParams
		{
			PagesList pages;
			GlyphsDictionary glyphs;
			KerningDictionary kerning;
			uint8_t space_width = 0;
			int height = 8;
		};

		explicit BitmapFont(const ConstructionParams& params);

		UC_NODISCARD size_t get_system_memory_use() const override;
		size_t get_used_resources(Set<Shared<Resource>>& resources) override;

		UC_NODISCARD float get_height() const override;

		Shared<Texture> get_char_print_info(uint32_t code,
			Vector2f& pos, Rectf* rect, Rectf* uv_rect) const override;

	protected:
		const List<Shared<Texture>> _pages;
		const GlyphsDictionary _glyphs;
		float _height;
	};
}