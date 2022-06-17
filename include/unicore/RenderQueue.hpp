#pragma once
#include "unicore/Rect.hpp"
#include "unicore/Render.hpp"

namespace unicore
{
	struct RenderCommand
	{
		RenderState state;
		size_t vstart = 0;
		size_t vcount = 0;
	};

	class RenderQueue
	{
	public:
		List<RenderCommand> commands;
		VertexList vertices;

		RenderQueue();

		void begin();
		void end();

		void flush();
		void draw(Render& render) const;

		RenderQueue& set_primitive_type(PrimitiveType type);
		RenderQueue& set_clip(const Optional<Recti>& rect);
		RenderQueue& set_texture(const Shared<Texture>& texture);

		RenderQueue& add_vertex(const Vertex& v);
		RenderQueue& add_vertices(const Vertex* data, size_t count);

	protected:
		RenderCommand _cmd;
	};
}