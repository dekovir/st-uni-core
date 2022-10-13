#pragma once
#include "unicore/renderer/Vertex.hpp"
#include "unicore/renderer/Texture.hpp"

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