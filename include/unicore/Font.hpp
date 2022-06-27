#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class Font : public Resource
	{
	public:

	protected:
		Dictionary<wchar_t, Dictionary<wchar_t, int>> _kerning;

		UC_NODISCARD int find_kerning(wchar_t a, wchar_t b) const
		{
			if (const auto it = _kerning.find(a); it != _kerning.end())
			{
				if (const auto jt = it->second.find(b); jt != it->second.end())
					return jt->second;
			}

			return 0;
		}
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

	protected:
		List<Shared<Texture>> _pages;
		Dictionary<wchar_t, BitmapFontGlyph> _glyphs;
	};

	class BitmapFontLoader : public ResourceLoaderT<BitmapFont>
	{
	public:
		UC_NODISCARD bool can_load_extension(WStringView ext) const override;
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}