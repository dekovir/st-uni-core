#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
	struct Vertex
	{
		Vector2f pos;
		Vector2f uv;
		Color4b col;

		static constexpr Vertex from_pos(const Vector2f& pos)
		{
			return { pos, Vector2f::Zero, Colors4b::White };
		}

		static constexpr Vertex from_pos(const Vector2i& pos)
		{
			return { pos.cast<float>(), Vector2f::Zero, Colors4b::White };
		}

		static constexpr Vertex from_pos(const Vector2f& pos, const Color4b& color)
		{
			return { pos, pos, color };
		}
	};

	using VertexList = List<Vertex>;
}