#include "unicore/Graphics.hpp"

namespace unicore
{
	void Graphics::begin()
	{
		clear();
	}

	void Graphics::end()
	{
		flush();
	}

	void Graphics::clear()
	{
		_batches.clear();
		_current = {};
	}

	void Graphics::to_render(Render2D& render) const
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

			case BatchType::Vertex:
				render.draw_triangles(&_vertices[batch.start], batch.count);
				break;

			default:
				UC_ASSERT_ALWAYS_MSG("Unimplemented BatchType");
				break;
			}
		}
	}

	Graphics& Graphics::set_color(const Color4b& color)
	{
		if (_current.color != color)
		{
			flush();
			_current.color = color;
		}
		return *this;
	}

	Graphics& Graphics::draw_point(const Vector2i& p)
	{
		set_type(BatchType::Point);

		_points.push_back(p.cast<float>());
		_current.count++;

		return *this;
	}

	Graphics& Graphics::draw_point(const Vector2f& p)
	{
		set_type(BatchType::Point);

		_points.push_back(p);
		_current.count++;

		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(p0.cast<float>());
		_points.push_back(p1.cast<float>());
		_current.count += 2;

		return *this;
	}

	Graphics& Graphics::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(p0);
		_points.push_back(p1);
		_current.count += 2;

		return *this;
	}

	Graphics& Graphics::draw_rect(const Recti& rect)
	{
		set_type(BatchType::Rect);
		_rects.push_back(rect.cast<float>());
		_current.count++;

		return *this;
	}

	Graphics& Graphics::draw_rect(const Rectf& rect)
	{
		set_type(BatchType::Rect);
		_rects.push_back(rect);
		_current.count++;

		return *this;
	}

	Graphics& Graphics::draw_tri(const Vertex2& v0, const Vertex2& v1, const Vertex2& v2)
	{
		set_type(BatchType::Vertex);
		_vertices.push_back(v0);
		_vertices.push_back(v1);
		_vertices.push_back(v2);
		_current.count += 3;
		return *this;
	}

	void Graphics::set_type(BatchType type)
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

	void Graphics::flush()
	{
		if (_current.count > 0)
		{
			_batches.push_back(_current);
			_current = {};
		}
	}
}