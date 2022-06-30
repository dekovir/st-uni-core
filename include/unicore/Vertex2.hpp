#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
	struct Vertex2
	{
		Vector2f pos;
		Vector2f uv;
		Color4b col;

		static constexpr Vertex2 from_pos(const Vector2f& pos)
		{
			return { pos, VectorConst2f::Zero, ColorConst4b::White };
		}

		static constexpr Vertex2 from_pos(const Vector2i& pos)
		{
			return { pos.cast<float>(), VectorConst2f::Zero, ColorConst4b::White };
		}

		static constexpr Vertex2 from_pos(const Vector2f& pos, const Color4b& color)
		{
			return { pos, pos, color };
		}
	};

	using VertexList2 = List<Vertex2>;
}