#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ConstString.hpp"
#include "unicore/ResourceFactory.hpp"

namespace unicore
{
	class Font : public Resource
	{
		UC_OBJECT(Font, Resource)
	public:
		UC_NODISCARD virtual float get_height() const = 0;

		UC_NODISCARD virtual float calc_width(WStringView text) const = 0;
		UC_NODISCARD virtual Vector2f calc_size(WStringView text) const;
	};

	class GeometryFont : public Font
	{
		UC_OBJECT(GeometryFont, Font)
	public:
		virtual size_t generate(const Vector2f& position, WStringView text,
			const Color4b& color, List<VertexColorQuad2>& quad_list) const = 0;
	};

	class TexturedFont : public Font
	{
		UC_OBJECT(TexturedFont, Font)
	public:
		virtual void generate(const Vector2f& position, WStringView text, const Color4b& color,
			Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict) = 0;
	};

	class TTFontOptions : public ResourceOptions
	{
	public:
		float height = 16;
		WStringView chars = CharTable::Ascii.view();

		TTFontOptions() = default;

		explicit TTFontOptions(float height_)
			: height(height_)
		{}

		TTFontOptions(float height_, WStringView chars_)
			: height(height_), chars(chars_)
		{}

		UC_NODISCARD size_t hash() const override
		{
			return Math::hash(height, chars);
		}
	};

	class TTFontFactory : public ResourceFactoryT<TexturedFont, TTFontOptions>
	{
		UC_OBJECT(TTFontFactory, ResourceFactory)
	public:
	};
}