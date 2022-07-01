#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
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
}