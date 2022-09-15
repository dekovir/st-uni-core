#include "unicore/Shapes.hpp"

namespace unicore::Shapes
{
	void gen_rect(List<Vector2f>& points, const Rectf& rect)
	{
		points.emplace_back(rect.x, rect.y);
		points.emplace_back(rect.x + rect.w, rect.y);
		points.emplace_back(rect.x + rect.w, rect.y + rect.h);
		points.emplace_back(rect.x, rect.y + rect.h);
	}

	List<Vector2f> gen_rect(const Rectf& rect)
	{
		List<Vector2f> points;
		gen_rect(points, rect);
		return points;
	}

	void gen_circle(List<Vector2f>& points, const Vector2f& center, float radius, unsigned segments)
	{
		if (radius == 0)
			return;

		if (segments == 0)
		{
			const float lng = 2 * Math::Pi * radius;
			segments = Math::max(3, Math::floor_to_int(lng / 10));
		}

		points.reserve(segments);
		for (unsigned i = 0; i < segments; i++)
		{
			const Radians angle = (360_deg / segments) * i;
			const auto cos = angle.cos();
			const auto sin = angle.sin();
			points.emplace_back(center.x + radius * cos, center.y + radius * sin);
		}
	}

	List<Vector2f> gen_circle(const Vector2f& center, float radius, unsigned segments)
	{
		List<Vector2f> points;
		gen_circle(points, center, radius, segments);
		return points;
	}

	void gen_ellipse(List<Vector2f>& points, const Vector2f& center, const Vector2f& radius, unsigned segments)
	{
		if (segments == 0)
		{
			const float a = Math::Pi * radius.x * radius.y + Math::pow(radius.x + radius.y);
			const float lng = 4 * (a / (radius.x - radius.y));
			segments = Math::max(3, Math::floor_to_int(lng / 100));
		}

		points.reserve(segments);
		for (unsigned i = 0; i < segments; i++)
		{
			const Radians angle = (360_deg / segments) * i;
			const auto cos = angle.cos();
			const auto sin = angle.sin();
			points.emplace_back(center.x + radius.x * cos, center.y + radius.y * sin);
		}
	}

	List<Vector2f> gen_ellipse(const Vector2f& center, const Vector2f& radius, unsigned segments)
	{
		List<Vector2f> points;
		gen_ellipse(points, center, radius, segments);
		return points;
	}

	void gen_star(List<Vector2f>& points, const Vector2f& center, unsigned count, float radius, float radius_inner)
	{
		if (count >= 2)
		{
			if (radius_inner <= 0)
				radius_inner = radius * .5f;

			const int segments = count * 2;
			const float step = 360.f / segments;
			points.reserve(segments + 2);

			for (int i = 0; i <= segments; i++)
			{
				const auto angle = -Degrees(180 + step * i);
				const float size = Math::even(i) ? radius : radius_inner;

				float sin, cos;
				angle.sin_cos(sin, cos);

				points.emplace_back(center.x + size * sin, center.y + size * cos);
			}
		}
	}

	List<Vector2f> gen_star(const Vector2f& center, unsigned count, float radius, float radius_inner)
	{
		List<Vector2f> points;
		gen_star(points, center, count, radius, radius_inner);
		return points;
	}
}