#pragma once
#include "unicore/Vector2.hpp"

namespace unicore
{
	class Font;

	enum class TextAlign : uint8_t
	{
		TopLeft, TopMiddle, TopRight,
		CenterLeft, Center, CenterRight,
		BottomLeft, BottomMiddle, BottomRight,
	};

	struct TextLine
	{
		WString text;
		Vector2f size = VectorConst2f::Zero;
		//Vector2f offset = VectorConst2f::Zero;
	};

	class TextBlock
	{
	public:
		TextBlock(const Shared<Font>& font, WStringView text);

		UC_NODISCARD const Shared<Font>& font() const { return _font; }
		UC_NODISCARD const List<TextLine>& lines() const { return _lines; }

		static void parse_lines(WStringView text_, List<TextLine>& lines);

		static void calc_line_size(const Font& font, List<TextLine>& lines);

		static Vector2f calc_align_offset(const Vector2f& size, TextAlign align);
		static Vector2f calc_align_offset(const Font& font, WStringView text, TextAlign align);

		static void calc_align_offset(const List<TextLine>& lines,
			TextAlign align, List<Vector2f>& offset_list);

	protected:
		Shared<Font> _font;
		List<TextLine> _lines;
	};

	class AlignedTextBlock : public TextBlock
	{
	public:
		AlignedTextBlock(const Shared<Font>& font, const WStringView& text, TextAlign align = TextAlign::TopLeft);

		UC_NODISCARD const List<Vector2f>& offset_list() const { return _offset_list; }

		void set_align(TextAlign align);
		UC_NODISCARD TextAlign align() const { return _align; }

	protected:
		List<Vector2f> _offset_list;
		TextAlign _align;

		void update_align();
	};
}