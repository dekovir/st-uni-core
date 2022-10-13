#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/math/Rect.hpp"
#include "unicore/math/Vector2.hpp"
#include "unicore/Vertex.hpp"
#include "unicore/Texture.hpp"

namespace unicore::sdl2
{
	enum class FlipBit
	{
		Horizontal = 1 << 0,
		Vertical = 1 << 1,
	};
	UNICORE_ENUM_FLAGS(FlipBit, RenderFlip);

	class PipelineRender
	{
	public:
		virtual ~PipelineRender() = default;

		// DRAW POINTS
		virtual void draw_point(const Vector2i& p) = 0;
		virtual void draw_point_f(const Vector2f& p) = 0;

		virtual void draw_points(const Vector2i* points, unsigned count) = 0;
		virtual void draw_points_f(const Vector2f* points, unsigned count) = 0;

		// DRAW LINES
		virtual void draw_line(const Vector2i& p1, const Vector2i& p2) = 0;
		virtual void draw_line_f(const Vector2f& p1, const Vector2f& p2) = 0;

		virtual void draw_poly_line(const Vector2i* points, unsigned count) = 0;
		virtual void draw_poly_line_f(const Vector2f* points, unsigned count) = 0;

		// DRAW RECTS
		virtual void draw_rect(const Recti& rect, bool filled) = 0;
		virtual void draw_rect_f(const Rectf& rect, bool filled) = 0;

		virtual void draw_rects(const Recti* rects, unsigned count, bool filled) = 0;
		virtual void draw_rects_f(const Rectf* rects, unsigned count, bool filled) = 0;

		// DRAW TRIANGLES
		virtual void draw_triangles(
			const VertexColor2* vertices, unsigned num_vertices
		) = 0;

		virtual void draw_triangles(
			const VertexTexColor2* vertices, unsigned num_vertices,
			const Texture* texture = nullptr
		) = 0;

		// COPY TEXTURE
		virtual bool copy(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect) = 0;

		virtual bool copy_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect) = 0;

		virtual bool copy_ex(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Recti>& dst_rect,
			Degrees angle, const Optional<Vector2i>& center, RenderFlip flip) = 0;

		virtual bool copy_ex_f(const Shared<Texture>& texture,
			const Optional<Recti>& src_rect, const Optional<Rectf>& dst_rect,
			Degrees angle, const Optional<Vector2f>& center, RenderFlip flip) = 0;
	};
}