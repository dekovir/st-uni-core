#pragma once
#include "unicore/Defs.hpp"
#include "unicore/Vertex2.hpp"

namespace unicore
{
	class Mesh
	{
	public:
		List<Vertex2> vertices;
		List<int> indices;

		void Clear()
		{
			vertices.clear();
			indices.clear();
		}
	};
}
