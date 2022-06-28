#include "unicore/Font.hpp"
#include "unicore/Data.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	bool BitmapFontLoader::can_load_extension(WStringView ext) const
	{
		return ext == L".fnt";
	}

	Shared<Resource> BitmapFontLoader::load(const ResourceLoaderContext& context)
	{
		const auto xml = context.cache.load<XMLData>(context.path, ResourceCacheFlag::SkipExtension);
		if (!xml)
		{
			UC_LOG_ERROR(context.logger) << "Failed to load xml";
			return nullptr;
		}

		Path texture_path(context.path);
		texture_path.replace_extension(L".bmp");
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
		font->_pages.push_back(texture);

		for (auto element = root->FirstChildElement("char"); element != nullptr; element = element->NextSiblingElement())
		{
			auto code = element->Int64Attribute("code");
			BitmapFontGlyph glyph{};

			glyph.rect.x = element->IntAttribute("x");
			glyph.rect.y = element->IntAttribute("y");
			glyph.rect.w = element->IntAttribute("w");
			glyph.rect.h = element->IntAttribute("h");
			glyph.xoffset = element->IntAttribute("dx");
			glyph.yoffset = element->IntAttribute("dy");
			glyph.xadvance = element->IntAttribute("advx");

			font->_glyphs.emplace(code, glyph);
		}

		for (auto element = root->FirstChildElement("kerning"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto c1 = element->IntAttribute("c1");
			const auto c2 = element->IntAttribute("c2");
			const auto offset = element->IntAttribute("offset");

			font->_kerning[c1][c2] = offset;
		}

		return font;
	}
}