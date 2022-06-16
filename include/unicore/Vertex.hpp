#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Color4.hpp"

namespace unicore
{
	struct Vertex
	{
		Vector2f pos;
		Color4b col;

		constexpr Vertex()
			: pos(0, 0), col(Colors4b::White)
		{}

		constexpr Vertex(const Vector2f& pos_)
			: pos(pos_), col(Colors4b::White)
		{}

		constexpr Vertex(const Vector2i& pos_)
			: pos(static_cast<float>(pos_.x), static_cast<float>(pos_.y)), col(Colors4b::White)
		{}

		constexpr Vertex(const Vector2f& pos_, const Color4b& col_)
			: pos(pos_), col(col_)
		{}

		constexpr Vertex(const Vector2i& pos_, const Color4b& col_)
			: pos(static_cast<float>(pos_.x), static_cast<float>(pos_.y)), col(col_)
		{}
	};

	using VertexList = List<Vertex>;
}