#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Angle.hpp"
#include "unicore/Render2D.hpp"

namespace unicore
{
	class Graphics
	{
	public:
		Graphics() = default;

		void begin();
		void end();

		void clear();
		void to_render(Render2D& render) const;

		Graphics& set_color(const Color4b& color);

		Graphics& draw_point(const Vector2i& p);
		Graphics& draw_point(const Vector2f& p);

		Graphics& draw_line(const Vector2i& p0, const Vector2i& p1);
		Graphics& draw_line(const Vector2f& p0, const Vector2f& p1);

		Graphics& draw_rect(const Recti& rect);
		Graphics& draw_rect(const Rectf& rect);

		Graphics& draw_tri(const Vertex2& v0, const Vertex2& v1, const Vertex2& v2);

	protected:
		enum class BatchType
		{
			Point,
			Line,
			Rect,
			Vertex
		};

		struct Batch
		{
			Color4b color = ColorConst4b::White;
			BatchType type = BatchType::Point;
			size_t start = 0;
			size_t count = 0;
		};

		List<Vector2f> _points;
		List<Rectf> _rects;
		VertexList _vertices;
		List<Batch> _batches;
		Batch _current;

		void set_type(BatchType type);

		void flush();
	};
}