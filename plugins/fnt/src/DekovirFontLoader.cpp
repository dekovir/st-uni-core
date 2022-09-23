#include "DekovirFontLoader.hpp"
#include "unicore/ResourceCache.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	DekovirFontLoader::DekovirFontLoader()
		: ResourceLoaderT({ L".fnt" })
	{
	}

	Shared<Resource> DekovirFontLoader::load(const Options& options)
	{
		const auto xml = options.cache.load<XMLData>(
			options.path, ResourceCacheFlag::IgnoreExtension);
		if (!xml)
		{
			UC_LOG_ERROR(options.logger) << "Failed to load xml";
			return nullptr;
		}

		Path texture_path(options.path);
		texture_path.replace_extension(Path::WildcardExt);

		const auto texture = options.cache.load<Texture>(texture_path);
		if (!texture)
		{
			UC_LOG_ERROR(options.logger) << "Failed to load texture";
			return nullptr;
		}

		const auto root = xml->doc.FirstChildElement("font");
		if (!root)
		{
			UC_LOG_ERROR(options.logger) << "Invalid Font format";
			return nullptr;
		}

		BitmapFont::ConstructionParams params;

		params.pages.push_back(texture);
		params.space_width = static_cast<uint8_t>(root->IntAttribute("spaceWidth"));
		params.height = root->IntAttribute("fontHeight");

		// TODO: Finish loading values
		for (auto element = root->FirstChildElement("char"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto code = static_cast<wchar_t>(element->Int64Attribute("code"));
			BitmapFontGlyph glyph{};

			glyph.rect.x = element->IntAttribute("x");
			glyph.rect.y = element->IntAttribute("y");
			glyph.rect.w = element->IntAttribute("w");
			glyph.rect.h = element->IntAttribute("h");
			glyph.xoffset = element->IntAttribute("dx");
			glyph.yoffset = element->IntAttribute("dy");
			glyph.xadvance = element->IntAttribute("advx");

			params.glyphs.insert_or_assign(code, glyph);
		}

		for (auto element = root->FirstChildElement("kerning"); element != nullptr; element = element->NextSiblingElement())
		{
			const auto c1 = element->IntAttribute("c1");
			const auto c2 = element->IntAttribute("c2");
			const auto offset = element->IntAttribute("offset");

			params.kerning[c1][c2] = offset;
		}

		return std::make_shared<BitmapFont>(params);
	}
}