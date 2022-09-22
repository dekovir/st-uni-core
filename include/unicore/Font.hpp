#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"
#include "unicore/ConstString.hpp"

namespace unicore
{
	class Font : public Resource
	{
		UC_OBJECT(Font, Resource)
	public:
		UC_NODISCARD virtual float get_height() const = 0;
		UC_NODISCARD virtual Vector2f calc_size(StringView text) const = 0;
	};

	class GeometryFont : public Font
	{
		UC_OBJECT(GeometryFont, Font)
	public:
		virtual size_t generate(const Vector2f& position, StringView text,
			List<VertexColorQuad2>& quad_list) const = 0;
	};

	class TexturedFont : public Font
	{
		UC_OBJECT(TexturedFont, Font)
	public:
		virtual void generate(const Vector2f& position, StringView text,
			Dictionary<Shared<Texture>, List<VertexTexColorQuad2>>& quad_dict) = 0;
	};
}