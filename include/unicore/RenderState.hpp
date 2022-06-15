#pragma once
#include "unicore/Texture.hpp"

namespace unicore
{
	enum class PrimitiveType
	{
		Points,
		LineList,
		TriangleList
	};

	struct RenderState
	{
		PrimitiveType primitiveType = PrimitiveType::TriangleList;
		Shared<Texture> texture = nullptr;
		Optional<Recti> clip_rect;
	};
}
