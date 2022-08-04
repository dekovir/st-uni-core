#pragma once
#include "unicore/Color4.hpp"
#include "unicore/Rect.hpp"
#include "unicore/Transform2.hpp"

namespace unicore
{
	class RendererSDL;

	class Graphics2D
	{
	public:
		Graphics2D() = default;

		Transform2 transform;

		void render(RendererSDL& renderer) const;

		Graphics2D& clear();
		Graphics2D& flush();

		Graphics2D& move(const Vector2f& move);
		Graphics2D& rotate(Radians angle);
		Graphics2D& set_transform(const Transform2& tr);
		Graphics2D& reset_transform();

		Graphics2D& set_color(const Color4b& color);

		Graphics2D& draw_point(const Vector2i& p);
		Graphics2D& draw_point(const Vector2f& p);

		Graphics2D& draw_line(const Vector2i& p0, const Vector2i& p1);
		Graphics2D& draw_line(const Vector2f& p0, const Vector2f& p1);

		Graphics2D& draw_poly_line(const List<Vector2f>& points, bool closed = false);

		Graphics2D& draw_rect(const Recti& rect, bool filled = false);
		Graphics2D& draw_rect(const Rectf& rect, bool filled = false);

		Graphics2D& draw_circle(const Vector2f& center, float radius, bool filled = false, unsigned segments = 0);

		Graphics2D& draw_triangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2);
		Graphics2D& draw_quad(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);

		Graphics2D& draw_convex_poly(const List<Vector2f>& points);

		Graphics2D& draw_grid(const Vector2i& count, const Vector2f& step,
		                      const Action<Graphics2D&, const Vector2f&>& draw_func, const Vector2f& offset = VectorConst2f::Zero);

	protected:
		enum class BatchType
		{
			Point,
			Line,
			Rect,
			RectFilled,
			Triangles,
		};

		struct Batch
		{
			Color4b color = ColorConst4b::White;
			BatchType type = BatchType::Point;
			size_t start = 0;
			size_t count = 0;
		};

		List<Vector2f> _points;
		List<Batch> _batches;
		Batch _current;

		void set_type(BatchType type);
	};
}