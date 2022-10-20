#pragma once
#include "unicore/renderer/Vertex.hpp"

namespace unicore
{
	// https://github.com/microsoft/DirectXTK/blob/main/Inc/GeometricPrimitive.h
	class GeometricPrimitive
	{
	public:
		using VertexType = VertexNormalColorTexture3f;
		using VertexCollection = List<VertexType>;
		using IndexCollection = List<UInt16>;

		// TODO: Implement
		static void create_cube(VertexCollection& vertices, IndexCollection& indeces, float size);

		// TODO: Implement
		static void create_box(VertexCollection& vertices, IndexCollection& indeces, const Vector3f& size);
	};
}