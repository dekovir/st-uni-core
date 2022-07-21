#pragma once
#include "unicore/Renderer.hpp"

namespace unicore
{
	class Surface;
	class Texture;

	class RendererBuffer : public RendererResource
	{
		UC_OBJECT(RendererBuffer, RendererResource)
	public:
		UC_NODISCARD virtual size_t size() const = 0;

		UC_NODISCARD size_t video_memory_use() const override { return elements_count() * element_size(); }

		UC_NODISCARD virtual size_t element_size() const = 0;
		UC_NODISCARD virtual size_t elements_count() const = 0;
	};

	class VertexBuffer : public RendererBuffer
	{
		UC_OBJECT(VertexBuffer, RendererBuffer)
	public:
	};

	class IndexBuffer : public RendererBuffer
	{
		UC_OBJECT(IndexBuffer, RendererBuffer)
	public:
	};

	template<typename TData, typename TBuffer>
	class RendererBufferHandler
	{
	public:
		static constexpr size_t ElementSize = sizeof(TData);

		explicit RendererBufferHandler(const Shared<TBuffer>& buffer)
			: _buffer(buffer)
		{}

		UC_NODISCARD const Shared<TBuffer>& buffer() const { return _buffer; }

	protected:
		Shared<TBuffer> _buffer;
	};

	template<typename TVertex>
	class VertexBufferHandler
	{
	public:
		static constexpr size_t VertexSize = sizeof(TVertex);

		explicit VertexBufferHandler(const Shared<VertexBuffer>& buffer)
			: _buffer(buffer)
		{}

		UC_NODISCARD const Shared<VertexBuffer>& buffer() const { return _buffer; }

		bool update(const TVertex* data, size_t count, size_t offset = 0)
		{
			return false;
		}

	protected:
		Shared<VertexBuffer> _buffer;
	};

	template<typename IndexType>
	class IndexBufferHandler
	{
	public:
		static constexpr size_t IndexSize = sizeof(IndexType);

		explicit IndexBufferHandler(const Shared<IndexBuffer>& buffer)
			: _buffer(buffer)
		{}

		bool update(const IndexType* data, size_t count, size_t offset = 0)
		{
			return false;
		}

	protected:
		Shared<IndexBuffer> _buffer;
	};

	class Renderer3D : public Renderer
	{
		UC_OBJECT(Renderer3D, Renderer)
	public:
	};
}