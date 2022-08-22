#include "BitmapFontLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	BitmapFontLoader::BitmapFontLoader()
		: ResourceLoaderT({ L".fnt" })
	{
	}

	Shared<Resource> BitmapFontLoader::load(const ResourceLoaderContext& context)
	{
		// TODO: Finish loading
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

		BitmapFont::PagesList pages;
		BitmapFont::GlyphsDictionary glyphs;
		BitmapFont::KerningDictionary kerning;

		pages.push_back(texture);

		auto space_width = static_cast<uint8_t>(root->IntAttribute("spaceWidth"));

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

			glyphs.insert_or_assign(code, glyph);
		}

		for (auto element = root->FirstChildElement("kerning"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto c1 = element->IntAttribute("c1");
			const auto c2 = element->IntAttribute("c2");
			const auto offset = element->IntAttribute("offset");

			kerning[c1][c2] = offset;
		}

		return std::make_shared<BitmapFont>(pages, glyphs, kerning, space_width);
	}
}