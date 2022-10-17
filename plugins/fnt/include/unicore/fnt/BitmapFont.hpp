#pragma once
#include "unicore/renderer/Font.hpp"
#include "unicore/math/Rect.hpp"

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
		using KerningDictionary = Dictionary<Char32, Dictionary<Char32, int>>;
		using PagesList = List<Shared<Texture>>;
		using GlyphsDictionary = Dictionary<Char32, BitmapFontGlyph>;

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
		UC_NODISCARD float calc_width(StringView32 text) const override;

		UC_NODISCARD int find_kerning(Char32 a, Char32 b) const;

		Shared<Texture> get_char_print_info(Char32 code,
			Vector2f& pos, Rectf* rect, Rectf* uv_rect) const;

		void generate(const Vector2f& position, StringView32 text, const Color4b& color,
			Dictionary<Shared<Texture>, List<QuadColorTexture2f>>& quad_dict) override;

	protected:
		const List<Shared<Texture>> _pages;
		const GlyphsDictionary _glyphs;
		const KerningDictionary _kerning;
		const float _height;
		const uint8_t _space_width = 0;

		Shared<Texture> print_char(Char32 code, Vector2f& pos,
			const Color4b& color, QuadColorTexture2f& quad) const;
	};
}