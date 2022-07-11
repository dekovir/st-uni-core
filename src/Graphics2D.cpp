#include "unicore/Graphics2D.hpp"

namespace unicore
{
	void Graphics2D::begin()
	{
		clear();
	}

	void Graphics2D::end()
	{
		flush();
	}

	void Graphics2D::clear()
	{
		offset = VectorConst2f::Zero;
		_batches.clear();
		_current = {};
	}

	void Graphics2D::render(Render2D& render) const
	{
		for (const auto& batch : _batches)
		{
			render.set_color(batch.color);
			switch (batch.type)
			{
			case BatchType::Point:
				render.draw_points_f(&_points[batch.start], batch.count);
				break;

			case BatchType::Line:
				render.draw_lines_f(&_points[batch.start], batch.count);
				break;

			case BatchType::Rect:
				render.draw_rects_f(&_rects[batch.start], batch.count);
				break;

			case BatchType::RectFilled:
				render.draw_rects_f(&_rects[batch.start], batch.count, true);
				break;

			case BatchType::Vertex:
				render.draw_triangles(&_vertices[batch.start], batch.count);
				break;

			default:
				UC_ASSERT_ALWAYS_MSG("Unimplemented BatchType");
				break;
			}
		}
	}

	Graphics2D& Graphics2D::set_color(const Color4b& color)
	{
		if (_current.color != color)
		{
			flush();
			_current.color = color;
		}
		return *this;
	}

	Graphics2D& Graphics2D::draw_point(const Vector2i& p)
	{
		set_type(BatchType::Point);

		_points.push_back(offset + p.cast<float>());
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_point(const Vector2f& p)
	{
		set_type(BatchType::Point);

		_points.push_back(offset + p);
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(offset + p0.cast<float>());
		_points.push_back(offset + p1.cast<float>());
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(offset + p0);
		_points.push_back(offset + p1);
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_poly_line(const List<Vector2f>& points, bool closed)
	{
		if (points.size() > 1)
		{
			set_type(BatchType::Line);

			for (unsigned i = 0; i + 1 < points.size(); i++)
			{
				_points.push_back(offset + points[i]);
				_points.push_back(offset + points[i + 1]);
				_current.count += 2;
			}

			if (closed)
			{
				_points.push_back(offset + points.back());
				_points.push_back(offset + points.front());
				_current.count += 2;
			}
		}

		return *this;
	}

	Graphics2D& Graphics2D::draw_rect(const Recti& rect, bool filled)
	{
		set_type(filled ? BatchType::RectFilled : BatchType::Rect);
		_rects.push_back(rect.cast<float>());
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_rect(const Rectf& rect, bool filled)
	{
		set_type(filled ? BatchType::RectFilled : BatchType::Rect);
		_rects.push_back(rect);
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_tri(
		const VertexColor2& v0, const VertexColor2& v1, const VertexColor2& v2)
	{
		set_type(BatchType::Vertex);
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 3;
		return *this;
	}

	Graphics2D& Graphics2D::draw_quad(
		const VertexColor2& v0, const VertexColor2& v1,
		const VertexColor2& v2, const VertexColor2& v3)
	{
		set_type(BatchType::Vertex);
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v3);
		_vertices.push_back(v3);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 6;
		return *this;
	}

	void Graphics2D::set_type(BatchType type)
	{
		if (_current.type != type)
		{
			flush();

			_current.type = type;
			switch (_current.type)
			{
			case BatchType::Point:
			case BatchType::Line:
				_current.start = _points.size();
				break;

			case BatchType::Rect:
			case BatchType::RectFilled:
				_current.start = _rects.size();
				break;

			case BatchType::Vertex:
				_current.start = _vertices.size();
				break;

			default:
				UC_ASSERT_ALWAYS_MSG("Unimplemented BatchType");
				break;
			}
		}
	}

	void Graphics2D::flush()
	{
		if (_current.count > 0)
		{
			_batches.push_back(_current);
			_current = {};
		}
	}
}