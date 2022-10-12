#pragma once
#include "Vertex.hpp"
#include "unicore/math/Rect.hpp"
#include "unicore/math/Transform2.hpp"
#include "unicore/Color4.hpp"
#include "unicore/TextBlock.hpp"

namespace unicore
{
	class GeometryFont;
	class RendererSDL;

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

	struct GraphicsLineStyle2D
	{
		float width = 1;
		float alignment = 0.5f;
		//GraphicsLineCap2D cap = GraphicsLineCap2D::Butt;
		//GraphicsLineJoin2D join = GraphicsLineJoin2D::Miter;
	};

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
		Graphics2D& scale(const Vector2f& value);
		Graphics2D& scale(Float value);

		Graphics2D& set_transform(const Transform2& tr);
		Graphics2D& reset_transform();

		Graphics2D& set_color(const Color4b& color);

		Graphics2D& draw_point(const Vector2i& p);
		Graphics2D& draw_point(const Vector2f& p);

		Graphics2D& draw_line(const Vector2i& p0, const Vector2i& p1);
		Graphics2D& draw_line(const Vector2f& p0, const Vector2f& p1);

		Graphics2D& draw_poly_line(const List<Vector2f>& points, bool closed = false);

		Graphics2D& draw_path(const List<Vector2f>& points, const GraphicsLineStyle2D& style, bool closed = false);

		Graphics2D& draw_rect(const Recti& rect, bool filled = false);
		Graphics2D& draw_rect(const Rectf& rect, bool filled = false);

		Graphics2D& draw_circle(const Vector2f& center, float radius, bool filled = false, unsigned segments = 0);
		Graphics2D& draw_ellipse(const Vector2f& center, const Vector2f& radius, bool filled = false, unsigned segments = 0);

		Graphics2D& draw_star(const Vector2f& center, unsigned count, float radius, bool filled = false);

		Graphics2D& draw_triangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2);
		Graphics2D& draw_quad(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3);
		Graphics2D& draw_quad(const VertexColorQuad2& quad);

		Graphics2D& draw_convex_poly(const List<Vector2f>& points);

		Graphics2D& draw_grid(const Vector2i& count, const Vector2f& step,
			const Action<Graphics2D&, const Vector2f&>& draw_func, const Vector2f& offset = VectorConst2f::Zero);

		Graphics2D& draw_text(const GeometryFont& font, const Vector2f& position,
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