#pragma once
#include "unicore/RenderQueue.hpp"

namespace unicore
{
	enum class GraphicsFlag
	{
		CallBegin = 1,
		CallEnd = 2,
		CallBeginEnd = CallBegin | CallEnd,
	};
	UNICORE_ENUMFLAGS(GraphicsFlag, GraphicsFlags);

	class Graphics
	{
	public:
		Graphics(RenderQueue& queue, GraphicsFlags flags = GraphicsFlags::Zero);
		~Graphics();

		Graphics& set_color(const Color& color);

		Graphics& draw_point(const Vector2i& p);
		Graphics& draw_point(const Vector2f& p);

		Graphics& draw_line(const Vector2i& p0, const Vector2i& p1);
		Graphics& draw_line(const Vector2f& p0, const Vector2f& p1);

		Graphics& draw_tri(
			const Vertex& v0, const Vertex& v1, const Vertex& v2,
			const Shared<Texture>& texture = nullptr);

		Graphics& draw_quad(
			const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3,
			const Shared<Texture>& texture = nullptr);

		Graphics& draw_sprite(const Shared<Texture>& texture, const Vector2f& center);

	protected:
		RenderQueue& _queue;
		Color _color;
		GraphicsFlags _flags;
	};
}