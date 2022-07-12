#pragma once
#include "unicore/Module.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Rect.hpp"
#include "unicore/RenderResource.hpp"

namespace unicore
{
	class Logger;
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

	class Render : public Module
	{
		UC_OBJECT(Render, Module)
	public:
		explicit Render(Logger& logger);

		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;
		UC_NODISCARD virtual uint32_t draw_calls() const =0;

		virtual Shared<Texture> create_texture(Surface& surface) = 0;
		virtual bool update_texture(Texture& texture, Surface& surface,
			Optional<Recti> rect = std::nullopt) = 0;

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;

	protected:
		Logger& _logger;
	};
}