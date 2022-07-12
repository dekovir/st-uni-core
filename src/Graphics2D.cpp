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
		_points.clear();
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
				render.draw_geometry(GeometryType::Points, &_points[batch.start], batch.count);
				break;

			case BatchType::Line:
				render.draw_geometry(GeometryType::LineList, &_points[batch.start], batch.count);
				break;

			case BatchType::Rect:
				render.draw_geometry(GeometryType::Rects, &_points[batch.start], batch.count);
				break;

			case BatchType::RectFilled:
				render.draw_geometry(GeometryType::RectsFilled, &_points[batch.start], batch.count);
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
		_points.push_back(offset + rect.position().cast<float>());
		_points.push_back(offset + rect.center().cast<float>());
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_rect(const Rectf& rect, bool filled)
	{
		set_type(filled ? BatchType::RectFilled : BatchType::Rect);
		_points.push_back(offset + rect.position());
		_points.push_back(offset + rect.center());
		_current.count += 2;

		return *this;
	}

	void Graphics2D::set_type(BatchType type)
	{
		if (_current.type != type)
		{
			flush();

			_current.type = type;
		}
	}

	void Graphics2D::flush()
	{
		if (_current.count > 0)
		{
			_batches.push_back(_current);

			_current = {};
			_current.start = _points.size();
		}
	}
}