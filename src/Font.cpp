#include "unicore/Font.hpp"
#include "unicore/Data.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	int Font::find_kerning(uint32_t a, uint32_t b) const
	{
		if (const auto it = kerning.find(a); it != kerning.end())
		{
			if (const auto jt = it->second.find(b); jt != it->second.end())
				return jt->second;
		}

		return 0;
	}

	Shared<Texture> BitmapFont::get_char_print_info(uint32_t code,
		Vector2f& pos, Rectf* rect, Rectf* uv_rect) const
	{
		if (const auto it = glyphs.find(code); it != glyphs.end())
		{
			const auto& c = it->second;
			if (c.page < pages.size())
			{
				auto page = pages[c.page];
				auto& size = page->size();

				if (uv_rect)
				{
					uv_rect->x = static_cast<float>(c.rect.x) / static_cast<float>(size.x);
					uv_rect->y = static_cast<float>(c.rect.y) / static_cast<float>(size.y);
					uv_rect->w = static_cast<float>(c.rect.w) / static_cast<float>(size.x);
					uv_rect->h = static_cast<float>(c.rect.h) / static_cast<float>(size.y);
				}

				if (rect)
				{
					rect->x = pos.x - static_cast<float>(c.xoffset);
					rect->y = pos.y + static_cast<float>(c.yoffset);
					rect->w = static_cast<float>(c.rect.w);
					rect->h = static_cast<float>(c.rect.h);
				}

				pos.x += static_cast<float>(c.xadvance);

				return page;
			}
		}

		return nullptr;
	}

	Shared<Texture> BitmapFont::print_char(uint32_t code, Vector2f& pos,
		VertexTexColor2& v0, VertexTexColor2& v1,
		VertexTexColor2& v2, VertexTexColor2& v3,
		const Color4b& color) const
	{
		if (code == 32)
		{
			pos.x += static_cast<float>(space_w);
			return nullptr;
		}

		if (const auto it = glyphs.find(code); it != glyphs.end())
		{
			const auto& c = it->second;
			if (c.page < pages.size())
			{
				auto page = pages[c.page];
				auto& size = page->size();

				const float tx1 = static_cast<float>(c.rect.x) / static_cast<float>(size.x);
				const float ty1 = static_cast<float>(c.rect.y) / static_cast<float>(size.y);
				const float tx2 = static_cast<float>(c.rect.max_x()) / static_cast<float>(size.x);
				const float ty2 = static_cast<float>(c.rect.max_y()) / static_cast<float>(size.y);

				const float x1 = pos.x - static_cast<float>(c.xoffset);
				const float y1 = pos.y + static_cast<float>(c.yoffset);
				const float x2 = x1 + static_cast<float>(c.rect.w);
				const float y2 = y1 + static_cast<float>(c.rect.h);

				v0.pos.set(x1, y1);
				v0.uv.set(tx1, ty1);
				v0.col = color;

				v1.pos.set(x2, y1);
				v1.uv.set(tx2, ty1);
				v1.col = color;

				v2.pos.set(x2, y2);
				v2.uv.set(tx2, ty2);
				v2.col = color;

				v3.pos.set(x1, y2);
				v3.uv.set(tx1, ty2);
				v3.col = color;

				pos.x += static_cast<float>(c.xadvance);

				return page;
			}
		}

		return nullptr;
	}

	Shared<Resource> BitmapFontLoader::load(const ResourceLoaderContext& context)
	{
		// TODO: Finish loading
#if defined(UNICORE_USE_XML)
		const auto xml = context.cache.load<XMLData>(context.path);
		if (!xml)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load xml";
			return nullptr;
		}

		Path texture_path(context.path);
		texture_path.replace_extension(L".*");

		const auto texture = context.cache.load<Texture>(texture_path);
		if (!texture)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load texture";
			return nullptr;
		}

		const auto root = xml->doc.FirstChildElement("font");
		if (!root)
		{
			UC_LOG_ERROR(context.logger) << "Invalid Font format";
			return nullptr;
		}

		auto font = make_shared<BitmapFont>();
		font->pages.push_back(texture);

		font->space_w = static_cast<uint8_t>(root->IntAttribute("spaceWidth"));

		for (auto element = root->FirstChildElement("char"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto code = (wchar_t)element->Int64Attribute("code");
			BitmapFontGlyph glyph{};

			glyph.rect.x = element->IntAttribute("x");
			glyph.rect.y = element->IntAttribute("y");
			glyph.rect.w = element->IntAttribute("w");
			glyph.rect.h = element->IntAttribute("h");
			glyph.xoffset = element->IntAttribute("dx");
			glyph.yoffset = element->IntAttribute("dy");
			glyph.xadvance = element->IntAttribute("advx");

			font->glyphs.insert_or_assign(code, glyph);
		}

		for (auto element = root->FirstChildElement("kerning"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto c1 = element->IntAttribute("c1");
			const auto c2 = element->IntAttribute("c2");
			const auto offset = element->IntAttribute("offset");

			font->kerning[c1][c2] = offset;
		}

		return font;
#else
		return nullptr;
#endif
	}
}