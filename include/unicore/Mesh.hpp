#pragma once
#include "unicore/Defs.hpp"
#include "unicore/Vertex.hpp"

namespace unicore
{
	class Mesh
	{
	public:
		List<VertexTexColor2> vertices;
		List<int> indices;

		void Clear()
		{
			vertices.clear();
			indices.clear();
		}
	};
}