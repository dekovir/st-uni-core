#pragma once
#include "Vertex.hpp"
#include "unicore/math/Rect.hpp"
#include "unicore/math/Transform2.hpp"
#include "unicore/system/TextBlock.hpp"
#include "unicore/renderer/Color4.hpp"
#include "unicore/renderer/sdl2/PipelineRender.hpp"

namespace unicore
{
	class GeometryFont;

	//enum class GraphicsLineCap2D
	//{
	//	Butt,
	//	Round,
	//	Square,
	//};

	//enum class GraphicsLineJoin2D
	//{
	//	Bevel,
	//	Miter,
	//	Round,
	//};

	struct PrimitiveBatchLineStyle
	{
		float width = 1;
		float alignment = 0.5f;
		//GraphicsLineCap2D cap = GraphicsLineCap2D::Butt;
		//GraphicsLineJoin2D join = GraphicsLineJoin2D::Miter;
	};

	class PrimitiveBatch
	{
	public:
		PrimitiveBatch() = default;

		Transform2f transform;

		void render(sdl2::PipelineRender& renderer) const;

		PrimitiveBatch& clear();
		PrimitiveBatch& flush();

		PrimitiveBatch& move(const Vector2f& move);
		PrimitiveBatch& rotate(Radians angle);
		PrimitiveBatch& scale(const Vector2f& value);
		PrimitiveBatch& scale(Float value);

		PrimitiveBatch& set_transform(const Transform2f& tr);
		PrimitiveBatch& reset_transform();

		PrimitiveBatch& set_color(const Color4b& color);

		PrimitiveBatch& draw_point(const Vector2i& p);
		PrimitiveBatch& draw_point(const Vector2f& p);

		PrimitiveBatch& draw_line(const Vector2i& p0, const Vector2i& p1);
		PrimitiveBatch& draw_line(const Vector2f& p0, const Vector2f& p1);

		PrimitiveBatch& draw_poly_line(const List<Vector2f>& points, bool closed = false);

		PrimitiveBatch& draw_path(const List<Vector2f>& points, const PrimitiveBatchLineStyle& style, bool closed = false);

		PrimitiveBatch& draw_rect(const Recti& rect, bool filled = false);
		PrimitiveBatch& draw_rect(const Rectf& rect, bool filled = false);

		PrimitiveBatch& draw_circle(const Vector2f& center, float radius, bool filled = false, unsigned segments = 0);
		PrimitiveBatch& draw_ellipse(const Vector2f& center, const Vector2f& radius, bool filled = false, unsigned segments = 0);

		PrimitiveBatch& draw_star(const Vector2f& center, unsigned count, float radius, bool filled = false);

		PrimitiveBatch& draw_curve(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, unsigned segments = 0);
		PrimitiveBatch& draw_curve(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3, unsigned segments = 0);

		// TODO: Replace with span
		PrimitiveBatch& draw_spline(const Vector2f* points, unsigned count, unsigned segments = 0);

		PrimitiveBatch& draw_triangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2);
		PrimitiveBatch& draw_quad(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);
		PrimitiveBatch& draw_quad(const QuadColor2f& quad);

		PrimitiveBatch& draw_convex_poly(const List<Vector2f>& points);

		PrimitiveBatch& draw_grid(const Vector2i& count, const Vector2f& step,
			const Action<PrimitiveBatch&, const Vector2f&>& draw_func, const Vector2f& offset = VectorConst2f::Zero);

		PrimitiveBatch& draw_text(const GeometryFont& font, const Vector2f& position,
			StringView32 text, TextAlign align = TextAlign::TopLeft);

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