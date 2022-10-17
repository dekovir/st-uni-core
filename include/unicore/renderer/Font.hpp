#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/system/ConstString.hpp"
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/renderer/Vertex.hpp"
#include "unicore/renderer/Texture.hpp"

namespace unicore
{
	class Font : public Resource
	{
		UC_OBJECT(Font, Resource)
	public:
		UC_NODISCARD virtual float get_height() const = 0;

		UC_NODISCARD virtual float calc_width(StringView32 text) const = 0;
		UC_NODISCARD virtual Vector2f calc_size(StringView32 text) const;
	};

	class GeometryFont : public Font
	{
		UC_OBJECT(GeometryFont, Font)
	public:
		virtual size_t generate(const Vector2f& position, StringView32 text,
			const Color4b& color, List<QuadColor2f>& quad_list) const = 0;
	};

	class TexturedFont : public Font
	{
		UC_OBJECT(TexturedFont, Font)
	public:
		virtual void generate(const Vector2f& position, StringView32 text, const Color4b& color,
			Dictionary<Shared<Texture>, List<QuadColorTexture2f>>& quad_dict) = 0;
	};

	class TTFontOptions : public ResourceOptions
	{
	public:
		float height = 16;
		// TODO: Replace to Set
		StringView32 chars = CharTable::Ascii.view();

		TTFontOptions() = default;

		explicit TTFontOptions(float height_)
			: height(height_)
		{}

		TTFontOptions(float height_, StringView32 chars_)
			: height(height_), chars(chars_)
		{}

		UC_NODISCARD size_t hash() const override
		{
			return Hash::make(height, chars);
		}
	};

	struct TTFontLoadPathPolicy : ResourceLoaderPathPolicy::Extension
	{
		explicit TTFontLoadPathPolicy()
			: Extension({ ".ttf" })
		{
		}
	};

	class TTFontFactory : public Resource
	{
		UC_OBJECT(TTFontFactory, Resource)
	public:
		virtual Shared<TexturedFont> create(
			const TTFontOptions& options, Logger* logger) = 0;
	};
}