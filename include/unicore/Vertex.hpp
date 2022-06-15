#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Color.hpp"

namespace unicore
{
	struct Vertex
	{
		Vector2f pos;
		Color col;

		constexpr Vertex()
			: pos(0, 0), col(Colors::White)
		{}

		constexpr Vertex(const Vector2f& pos_)
			: pos(pos_), col(Colors::White)
		{}

		constexpr Vertex(const Vector2i& pos_)
			: pos(static_cast<float>(pos_.x), static_cast<float>(pos_.y)), col(Colors::White)
		{}

		constexpr Vertex(const Vector2f& pos_, const Color& col_)
			: pos(pos_), col(col_)
		{}

		constexpr Vertex(const Vector2i& pos_, const Color& col_)
			: pos(static_cast<float>(pos_.x), static_cast<float>(pos_.y)), col(col_)
		{}
	};

	using VertexList = List<Vertex>;
}