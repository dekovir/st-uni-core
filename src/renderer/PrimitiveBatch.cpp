#include "unicore/renderer/PrimitiveBatch.hpp"
#include "unicore/math/ShapePrimitive.hpp"
#include "unicore/renderer/Font.hpp"

namespace unicore
{
	static List<Vector2f> s_points;
	static List<VertexColor2f> s_verts;
	static List<QuadColor2f> s_quads;

	void PrimitiveBatch::render(sdl2::PipelineRender& renderer) const
	{
		for (const auto& batch : _batches)
		{
			renderer.set_draw_color(batch.color);
			switch (batch.type)
			{
			case BatchType::Point:
				renderer.draw_pointsf(&_points[batch.start], batch.count);
				break;

			case BatchType::Line:
				for (unsigned i = 0; i + 1 < batch.count; i += 2)
				{
					const auto& p0 = _points[batch.start + i + 0];
					const auto& p1 = _points[batch.start + i + 1];
					renderer.draw_linef(p0, p1);
				}
				break;

			case BatchType::Rect:
			case BatchType::RectFilled:
				for (unsigned i = 0; i + 1 < batch.count; i += 2)
				{
					const auto& p0 = _points[batch.start + i + 0];
					const auto& p1 = _points[batch.start + i + 1];
					const Rectf r(p0, p1);
					renderer.draw_rectf(r, batch.type == BatchType::RectFilled);
				}
				break;

			case BatchType::Triangles:
				s_verts.resize(batch.count);
				for (unsigned i = 0; i < batch.count; i++)
				{
					s_verts[i].pos = _points[batch.start + i];
					s_verts[i].col = batch.color;
				}
				renderer.draw_trianglesf(s_verts.data(), batch.count);
				break;

			default:
				UC_ASSERT_ALWAYS_MSG("Unimplemented BatchType");
				break;
			}
		}
	}

	PrimitiveBatch& PrimitiveBatch::clear()
	{
		transform.clear();

		_points.clear();
		_batches.clear();
		_current = {};

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::flush()
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

	PrimitiveBatch& PrimitiveBatch::move(const Vector2f& move)
	{
		transform *= Transform2f::moved(move);
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::rotate(Radians angle)
	{
		transform *= Transform2f::rotated(angle);
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::scale(const Vector2f& value)
	{
		transform *= Transform2f::scaled(value);
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::scale(Float value)
	{
		transform *= Transform2f::scaled(value);
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::set_transform(const Transform2f& tr)
	{
		transform = tr;
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::reset_transform()
	{
		transform.clear();
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::set_color(const Color4b& color)
	{
		if (_current.color != color)
		{
			flush();
			_current.color = color;
		}
		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_point(const Vector2i& p)
	{
		set_type(BatchType::Point);

		_points.push_back(transform * p.cast<float>());
		_current.count++;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_point(const Vector2f& p)
	{
		set_type(BatchType::Point);

		_points.push_back(transform * p);
		_current.count++;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_line(const Vector2i& p0, const Vector2i& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(transform * p0.cast<float>());
		_points.push_back(transform * p1.cast<float>());
		_current.count += 2;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_line(const Vector2f& p0, const Vector2f& p1)
	{
		set_type(BatchType::Line);

		_points.push_back(transform * p0);
		_points.push_back(transform * p1);
		_current.count += 2;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_poly_line(const List<Vector2f>& points, bool closed)
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

	struct PathEdge
	{
		unsigned i0, i1;
		Vector2f dir, perpendicular;

		PathEdge(const List<Vector2f>& points, unsigned index)
			: i0(index), i1((index + 1) % points.size())
			, dir((points[i1] - points[i0]).normalized())
			, perpendicular(dir.perpendicular())
		{
		}
	};

	static List<PathEdge> s_path;

	static int clamp_path_index(const List<PathEdge>& path, int index, bool closed)
	{
		const int count = static_cast<int>(path.size());
		if (index < 0)
		{
			if (closed)
			{
				return index + count;
			}
		}
		else if (index >= count)
		{
			return index % count;
		}

		return index;
	}

	static void draw_path_segment(PrimitiveBatch& graphics, const List<Vector2f>& points,
		const GraphicsLineStyle2D& style, const List<PathEdge>& path, unsigned index, bool closed)
	{
		auto& edge = path[index];
		const auto p0 = points[edge.i0];
		const auto p1 = points[edge.i1];

		const auto outer_a = Math::clamp_01(style.alignment);
		const auto inner_a = 1 - outer_a;

		graphics.draw_line(p0, p1);

		Vector2f v0, v1, v2, v3;

		if (const auto prev_edge = clamp_path_index(path, static_cast<int>(index) - 1, closed); prev_edge >= 0)
		{
			auto& prev = path[prev_edge];
			const auto p = (edge.perpendicular + prev.perpendicular).normalized();

			v0 = p0 + p * style.width * outer_a;
			v3 = p0 - p * style.width * inner_a;
		}
		else
		{
			v0 = p0 + edge.perpendicular * style.width * outer_a;
			v3 = p0 - edge.perpendicular * style.width * inner_a;
		}

		if (const auto next_edge = clamp_path_index(path, static_cast<int>(index) + 1, closed); next_edge >= 0)
		{
			auto& next = path[next_edge];
			const auto p = (edge.perpendicular + next.perpendicular).normalized();

			v1 = p1 + p * style.width * outer_a;
			v2 = p1 - p * style.width * inner_a;
		}
		else
		{
			v1 = p1 + edge.perpendicular * style.width * outer_a;
			v2 = p1 - edge.perpendicular * style.width * inner_a;
		}

		graphics.draw_quad(v0, v1, v2, v3);

		//graphics.draw_line(p0, p0_edge);
		//graphics.draw_line(p1, p1_edge);
	}

	PrimitiveBatch& PrimitiveBatch::draw_path(const List<Vector2f>& points, const GraphicsLineStyle2D& style, bool closed)
	{
		if (points.size() >= 2)
		{
			s_path.clear();

			for (unsigned i = 0; i + 1 < points.size(); i++)
				s_path.emplace_back(points, i);

			if (closed)
				s_path.emplace_back(points, points.size() - 1);

			for (unsigned i = 0; i < s_path.size(); i++)
				draw_path_segment(*this, points, style, s_path, i, closed);
		}

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_rect(const Recti& rect, bool filled)
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

	PrimitiveBatch& PrimitiveBatch::draw_rect(const Rectf& rect, bool filled)
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

	PrimitiveBatch& PrimitiveBatch::draw_circle(const Vector2f& center, float radius, bool filled, unsigned segments)
	{
		if (radius == 0)
			return draw_point(center);

		s_points.clear();
		ShapePrimitive::create_circle(s_points, center, radius, segments);

		return !filled
			? draw_poly_line(s_points, true)
			: draw_convex_poly(s_points);
	}

	PrimitiveBatch& PrimitiveBatch::draw_ellipse(const Vector2f& center, const Vector2f& radius, bool filled, unsigned segments)
	{
		s_points.clear();
		ShapePrimitive::create_ellipse(s_points, center, radius, segments);

		return !filled
			? draw_poly_line(s_points, true)
			: draw_convex_poly(s_points);
	}

	PrimitiveBatch& PrimitiveBatch::draw_star(const Vector2f& center, unsigned count, float radius, bool filled)
	{
		if (count >= 2)
		{
			s_points.clear();
			ShapePrimitive::create_star(s_points, center, count, radius);

			if (filled)
				s_points.insert(s_points.begin(), center);

			return !filled
				? draw_poly_line(s_points, true)
				: draw_convex_poly(s_points);
		}

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_triangle(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2)
	{
		set_type(BatchType::Triangles);

		_points.push_back(transform * p0);
		_points.push_back(transform * p1);
		_points.push_back(transform * p2);
		_current.count += 3;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_quad(const Vector2f& p0, const Vector2f& p1, const Vector2f& p2, const Vector2f& p3)
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

	PrimitiveBatch& PrimitiveBatch::draw_quad(const QuadColor2f& quad)
	{
		set_type(BatchType::Triangles);

		_points.push_back(transform * quad.v[0].pos);
		_points.push_back(transform * quad.v[1].pos);
		_points.push_back(transform * quad.v[2].pos);
		_points.push_back(transform * quad.v[0].pos);
		_points.push_back(transform * quad.v[2].pos);
		_points.push_back(transform * quad.v[3].pos);
		_current.count += 6;

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_convex_poly(const List<Vector2f>& points)
	{
		for (unsigned i = 0; i + 2 < points.size(); i += 1)
			draw_triangle(points[0], points[i + 1], points[i + 2]);

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_grid(const Vector2i& count, const Vector2f& step,
		const Action<PrimitiveBatch&, const Vector2f&>& draw_func, const Vector2f& offset)
	{
		for (auto y = 0; y < count.y; y++)
			for (auto x = 0; x < count.x; x++)
			{
				const auto center = offset + step * Vector2f(x, y);
				draw_func(*this, center);
			}

		return *this;
	}

	PrimitiveBatch& PrimitiveBatch::draw_text(const GeometryFont& font,
		const Vector2f& position, StringView32 text, TextAlign align)
	{
		if (align == TextAlign::TopLeft)
		{
			s_quads.clear();
			const auto count = font.generate(position, text, _current.color, s_quads);
			for (unsigned i = 0; i < count; i++)
				draw_quad(s_quads[i]);
		}
		else
		{
			static List<TextLine> lines;
			static List<Vector2f> offsets;

			lines.clear();
			TextBlock::parse_lines(text, lines);
			TextBlock::calc_line_size(font, lines);

			TextBlock::calc_align_offset(lines, align, offsets);

			for (unsigned i = 0; i < lines.size(); i++)
				draw_text(font, position + offsets[i], lines[i].text);
		}

		return *this;
	}

	// ============================================================================
	void PrimitiveBatch::set_type(BatchType type)
	{
		if (_current.type != type)
		{
			flush();

			_current.type = type;
		}
	}
}