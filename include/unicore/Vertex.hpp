#pragma once
#include "unicore/math/Vector2.hpp"
#include "unicore/math/Vector3.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
	template<typename TVertex>
	struct Quad
	{
		TVertex v[4];
	};

	template<typename TPos, typename TColor>
	struct VertexColor
	{
		TPos pos;
		TColor col;

		constexpr VertexColor() = default;

		constexpr VertexColor(const TPos& pos_, const TColor& color_)
			: pos(pos_), col(color_)
		{}
	};

	using VertexColor2 = VertexColor<Vector2f, Color4b>;
	using VertexColor3 = VertexColor<Vector3f, Color4b>;

	using VertexColorQuad2 = Quad<VertexColor2>;
	using VertexColorQuad3 = Quad<VertexColor3>;

	template<typename TPos, typename TTexCoords, typename TColor>
	struct VertexTexColor
	{
		TPos pos;
		TTexCoords uv;
		TColor col;

		constexpr VertexTexColor() = default;

		constexpr VertexTexColor(const TPos& pos_, const TColor& color_)
			: pos(pos_), uv({}), col(color_)
		{}

		constexpr VertexTexColor(const TPos& pos_, const TTexCoords& uv_, const TColor& color_)
			: pos(pos_), uv(uv_), col(color_)
		{}
	};

	using VertexTexColor2 = VertexTexColor<Vector2f, Vector2f, Color4b>;
	using VertexTexColor3 = VertexTexColor<Vector3f, Vector2f, Color4b>;

	using VertexTexColorQuad2 = Quad<VertexTexColor2>;
	using VertexTexColorQuad3 = Quad<VertexTexColor3>;
}