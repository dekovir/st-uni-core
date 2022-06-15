#pragma once
#include "unicore/PrimitiveType.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	struct RenderState
	{
		PrimitiveType primitiveType = PrimitiveType::TriangleList;
		Shared<Texture> texture = nullptr;
		Optional<Recti> clip_rect;
	};
}
