#pragma once
#include "unicore/Defs.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	template<typename TVertex>
	class Mesh
	{
	public:
		List<TVertex> vertices;
		List<int> indices;
		List<Shared<Texture>> textures;

		void clear()
		{
			vertices.clear();
			indices.clear();
		}
	};

	using Mesh2 = Mesh<VertexTexColor2>;
}