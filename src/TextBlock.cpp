#include "unicore/TextBlock.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	TextBlock::TextBlock(const Shared<Font>& font, WStringView text)
		: _font(font)
	{
		parse_lines(text, _lines);
		calc_line_size(*_font, _lines);
	}

	void TextBlock::parse_lines(WStringView text_, List<TextLine>& lines)
	{
		WStringView text = text_;
		size_t pos;
		while ((pos = text.find_first_of(L'\n')) != WStringView::npos)
		{
			if (pos > 0)
				lines.push_back({ WString(text.substr(0, pos)) });
			else lines.push_back({ L"" });
			text = text.substr(pos + 1);
		}

		if (!text.empty())
			lines.push_back({ WString(text) });
	}

	void TextBlock::calc_line_size(const Font& font, List<TextLine>& lines)
	{
		for (auto& line : lines)
			line.size = !line.text.empty() ? font.calc_size(line.text) : Vector2f(0, font.get_height());
	}

	Vector2f TextBlock::calc_align_offset(const Vector2f& size, TextAlign align)
	{
		Vector2f offset;

		// X
		switch (align)
		{
		case TextAlign::TopMiddle:
		case TextAlign::Center:
		case TextAlign::BottomMiddle:
			offset.x = -size.x / 2;
			break;

		case TextAlign::CenterRight:
		case TextAlign::TopRight:
		case TextAlign::BottomRight:
			offset.x = -size.x;
			break;

		default:
			offset.x = 0;
			break;
		}

		// Y
		switch (align)
		{
		case TextAlign::CenterLeft:
		case TextAlign::Center:
		case TextAlign::CenterRight:
			offset.y = -size.y / 2;
			break;

		case TextAlign::BottomLeft:
		case TextAlign::BottomMiddle:
		case TextAlign::BottomRight:
			offset.y = -size.y;
			break;

		default:
			offset.y = 0;
			break;
		}

		return offset;
	}

	Vector2f TextBlock::calc_align_offset(const Font& font, WStringView text, TextAlign align)
	{
		const auto size = font.calc_size(text);
		return calc_align_offset(size, align);
	}

	void TextBlock::calc_align_offset(const List<TextLine>& lines,
		TextAlign align, List<Vector2f>& offset_list)
	{
		const auto total_size = std::accumulate(lines.begin(), lines.end(),
			VectorConst2f::Zero, [](Vector2f size, const TextLine& item)
			{
				size.y += item.size.y;
				size.x = Math::max(size.x, item.size.x);
				return size;
			});

		auto total_offset = calc_align_offset(total_size, align);

		offset_list.clear();
		offset_list.reserve(lines.size());

		for (const auto& line : lines)
		{
			offset_list.push_back(total_offset);
			total_offset.y += line.size.y;
		}
	}

	// AlignedTextBlock ///////////////////////////////////////////////////////////
	AlignedTextBlock::AlignedTextBlock(const Shared<Font>& font, const WStringView& text, TextAlign align)
		: TextBlock(font, text)
		, _align(align)
	{
		update_align();
	}

	void AlignedTextBlock::set_align(TextAlign align)
	{
		if (_align == align) return;

		_align = align;
		update_align();
	}

	void AlignedTextBlock::update_align()
	{
		calc_align_offset(_lines, _align, _offset_list);
	}
}