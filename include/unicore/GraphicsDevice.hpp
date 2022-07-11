#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	enum class PrimitiveType
	{
		LineList,
		//LineStrip,
		PointList,
		TriangleList,
		//TriangleStrip,
	};

	class GraphicsDevice : public Object
	{
		UC_OBJECT(GraphicsDevice, Object)
	public:
		virtual void draw_primitives(PrimitiveType type, uint32_t vertex_start) = 0;
	};
}