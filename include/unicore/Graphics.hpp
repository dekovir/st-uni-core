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
		explicit Graphics(RenderQueue& queue, GraphicsFlags flags = GraphicsFlags::Zero);
		~Graphics();

		Graphics& set_color(const Color4b& color);

		Graphics& draw_point(const Vector2i& p);
		Graphics& draw_point(const Vector2f& p);

		Graphics& draw_line(const Vector2i& p0, const Vector2i& p1);
		Graphics& draw_line(const Vector2f& p0, const Vector2f& p1);

		Graphics& draw_poly_line(std::initializer_list<Vector2f> points);

		Graphics& draw_tri(
			const Vertex& v0, const Vertex& v1, const Vertex& v2,
			const Shared<Texture>& texture = nullptr);

		Graphics& draw_quad(
			const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3,
			const Shared<Texture>& texture = nullptr);

		Graphics& draw_sprite_at(const Shared<Texture>& texture, const Vector2f& center);
		Graphics& draw_sprite_at(const Shared<Texture>& texture,
			const Vector2f& center, const Radians& angle, const Vector2f& scale = Vector2fConst::One);

	protected:
		RenderQueue& _queue;
		Color4b _color;
		GraphicsFlags _flags;

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);

		static void calc_quad_position(const Vector2f& center, const Vector2i& size,
			const Radians& angle, const Vector2f& scale,
			Vector2f& p0, Vector2f& p1, Vector2f& p2, Vector2f& p3);
	};
}