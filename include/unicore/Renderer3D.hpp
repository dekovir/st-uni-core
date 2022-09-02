#pragma once
#include "unicore/Renderer.hpp"
#include "unicore/RendererResource.hpp"

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

	enum class VertexElementType : uint8_t
	{
		Position,
		Normal,
		Color0,
		Texture0,
		Count,
	};

	enum class VertexElementValue : uint8_t
	{
		Byte4,
		Float2,
		Float4,
	};

	struct VertexElement
	{
		VertexElementValue value;
		uint16_t offset;
	};

	class VertexLayout
	{
	public:
		using ArrayType = Array<VertexElement, static_cast<int>(VertexElementType::Count)>;

		VertexLayout(const ArrayType& elements_, uint16_t stride_)
			: elements(elements_), stride(stride_)
		{}

		const ArrayType elements;
		uint16_t stride;

		static constexpr size_t get_value_size(VertexElementValue value)
		{
			switch (value)
			{
			case VertexElementValue::Byte4: return sizeof(uint8_t) * 4;
			case VertexElementValue::Float2: return sizeof(float) * 2;
			case VertexElementValue::Float4: return sizeof(float) * 4;
			}
			return 0;
		}
	};

	class VertexLayoutBuilder
	{
	public:
		VertexLayoutBuilder()
			: _elements({ VertexElementValue::Byte4, 0 })
			, _stride(0)
		{
		}

		VertexLayoutBuilder& add(VertexElementType type, VertexElementValue value, uint16_t offset)
		{
			_elements[static_cast<int>(type)] = { value, offset };
		}

		VertexLayout build()
		{
			return { _elements, _stride };
		}

	protected:
		VertexLayout::ArrayType _elements;
		uint16_t _stride;
	};

	class RendererBuffer : public RendererResource
	{
		UC_OBJECT(RendererBuffer, RendererResource)
	public:
		UC_NODISCARD virtual size_t size() const = 0;

		UC_NODISCARD size_t get_video_memory_use() const override { return elements_count() * element_size(); }

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