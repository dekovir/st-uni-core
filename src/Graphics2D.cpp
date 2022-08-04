#include "unicore/Graphics2D.hpp"
#include "unicore/RendererSDL.hpp"

namespace unicore
{
	static List<Vector2f> s_points;
	static List<VertexColor2> s_verts;

	void Graphics2D::render(RendererSDL& renderer) const
	{
		for (const auto& batch : _batches)
		{
			renderer.set_draw_color(batch.color);
			switch (batch.type)
			{
			case BatchType::Point:
				renderer.draw_points_f(&_points[batch.start], batch.count);
				break;

			case BatchType::Line:
				for (unsigned i = 0; i + 1 < batch.count; i += 2)
				{
					const auto& p0 = _points[batch.start + i + 0];
					const auto& p1 = _points[batch.start + i + 1];
					renderer.draw_line_f(p0, p1);
				}
				break;

			case BatchType::Rect:
			case BatchType::RectFilled:
				for (unsigned i = 0; i + 1 < batch.count; i += 2)
				{
					const auto& p0 = _points[batch.start + i + 0];
					const auto& p1 = _points[batch.start + i + 1];
					const Rectf r(p0, p1);
					renderer.draw_rect_f(r, batch.type == BatchType::RectFilled);
				}
				break;

			case BatchType::Triangles:
				s_verts.resize(batch.count);
				for (unsigned i = 0; i < batch.count; i++)
				{
					s_verts[i].pos = _points[batch.start + i];
					s_verts[i].col = batch.color;
				}
				renderer.draw_triangles(s_verts.data(), batch.count);
				break;

			default:
				UC_ASSERT_ALWAYS_MSG("Unimplemented BatchType");
				break;
			}
		}
	}

	Graphics2D& Graphics2D::clear()
	{
		transform.clear();

		_points.clear();
		_batches.clear();
		_current = {};

		return *this;
	}

	Graphics2D& Graphics2D::flush()
	{
		if (_current.count > 0)
		{
			const auto color = _current.color;
			_batches.push_back(_current);

			_current = {};
			_current.color = color;
			_current.start = _points.size();
		}

		return *this;
	}

	Graphics2D& Graphics2D::move(const Vector2f& move)
	{
		transform *= Transform2::moved(move);
		return *this;
	}

	Graphics2D& Graphics2D::rotate(Radians angle)
	{
		transform *= Transform2::rotated(angle);
		return *this;
	}

	Graphics2D& Graphics2D::set_transform(const Transform2& tr)
	{
		transform = tr;
		return *this;
	}

	Graphics2D& Graphics2D::reset_transform()
	{
		transform.clear();
		return *this;
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

		_points.push_back(transform * p.cast<float>());
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_point(const Vector2f& p)
	{
		set_type(BatchType::Point);

		_points.push_back(transform * p);
		_current.count++;

		return *this;
	}

	Graphics2D& Graphics2D::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(transform * p0.cast<float>());
		_points.push_back(transform * p1.cast<float>());
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(transform * p0);
		_points.push_back(transform * p1);
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
				_points.push_back(transform * points[i]);
				_points.push_back(transform * points[i + 1]);
				_current.count += 2;
			}

			if (closed)
			{
				_points.push_back(transform * points.back());
				_points.push_back(transform * points.front());
				_current.count += 2;
			}
		}

		return *this;
	}

	Graphics2D& Graphics2D::draw_rect(const Recti& rect, bool filled)
	{
		set_type(filled ? BatchType::RectFilled : BatchType::Rect);

		const auto r = Rectf::from_min_max(
			transform * rect.bottom_left().cast<float>(),
			transform * rect.top_right().cast<float>()
		);

		_points.push_back(r.position());
		_points.push_back(r.size());
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_rect(const Rectf& rect, bool filled)
	{
		set_type(filled ? BatchType::RectFilled : BatchType::Rect);

		const auto r = Rectf::from_min_max(
			transform * rect.bottom_left(),
			transform * rect.top_right()
		);

		_points.push_back(r.position());
		_points.push_back(r.size());
		_current.count += 2;

		return *this;
	}

	Graphics2D& Graphics2D::draw_circle(const Vector2f& center, float radius, bool filled, unsigned segments)
	{
		if (radius == 0)
			return draw_point(center);

		if (segments == 0)
			segments = Math::max(5, Math::floor(radius * .7f));

		s_points.resize(segments);
		for (unsigned i = 0; i < segments; i++)
		{
			const Radians angle = (360_deg / segments) * i;
			const auto cos = angle.cos();
			const auto sin = angle.sin();
			s_points[i] = Vector2f(center.x + radius * cos, center.y + radius * sin);
		}

		return !filled
			? draw_poly_line(s_points, true)
			: draw_convex_poly(s_points);
	}

	Graphics2D& Graphics2D::draw_triangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2)
	{
		set_type(BatchType::Triangles);

		_points.push_back(transform * p0);
		_points.push_back(transform * p1);
		_points.push_back(transform * p2);
		_current.count += 3;

		return *this;
	}

	Graphics2D& Graphics2D::draw_quad(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3)
	{
		set_type(BatchType::Triangles);

		_points.push_back(transform * p0);
		_points.push_back(transform * p1);
		_points.push_back(transform * p2);

		_points.push_back(transform * p0);
		_points.push_back(transform * p2);
		_points.push_back(transform * p3);
		_current.count += 6;

		return *this;
	}

	Graphics2D& Graphics2D::draw_convex_poly(const List<Vector2f>& points)
	{
		for (unsigned i = 0; i + 2 < points.size(); i += 1)
			draw_triangle(points[0], points[i + 1], points[i + 2]);

		return *this;
	}

	Graphics2D& Graphics2D::draw_grid(const Vector2i& count, const Vector2f& step,
		const Action<Graphics2D&, const Vector2f&>& draw_func, const Vector2f& offset)
	{
		for (auto y = 0; y < count.y; y++)
			for (auto x = 0; x < count.x; x++)
			{
				const auto center = offset + step * Vector2f(x, y);
				draw_func(*this, center);
			}

		return *this;
	}

	// ============================================================================
	void Graphics2D::set_type(BatchType type)
	{
		if (_current.type != type)
		{
			flush();

			_current.type = type;
		}
	}
}