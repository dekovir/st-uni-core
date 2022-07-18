#pragma once
#include "unicore/Renderer.hpp"

namespace unicore
{
	class Surface;
	class Texture;

	class RenderBuffer : public RenderResource
	{
		UC_OBJECT(RenderBuffer, RenderResource)
	public:
		UC_NODISCARD virtual size_t size() const = 0;

		UC_NODISCARD virtual size_t item_size() const = 0;
		UC_NODISCARD virtual size_t item_count() const = 0;
	};

	class VertexBuffer : public RenderBuffer
	{
		UC_OBJECT(VertexBuffer, RenderBuffer)
	public:
	};

	class IndexBuffer : public RenderBuffer
	{
		UC_OBJECT(IndexBuffer, RenderBuffer)
	public:
	};

	class Renderer2D : public Renderer
	{
		UC_OBJECT(Renderer2D, Renderer)
	public:
	};
}