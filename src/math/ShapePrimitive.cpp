#include "unicore/math/ShapePrimitive.hpp"
#include "unicore/math/Curve.hpp"

namespace unicore
{
	void ShapePrimitive::rect(List<Vector2f>& out, const Rectf& value)
	{
		value.push_points(out);
	}

	List<Vector2f> ShapePrimitive::rect(const Rectf& value)
	{
		List<Vector2f> out;
		rect(out, value);
		return out;
	}

	// CIRCLE ////////////////////////////////////////////////////////////////////
	void ShapePrimitive::circle(List<Vector2f>& out,
		const Vector2f& center, float radius, unsigned segments)
	{
		if (Math::equals(radius, 0.0f))
			return;

		if (segments == 0)
		{
			const float lng = 2 * Math::Pi * radius;
			segments = Math::max(3, Math::floor_to_int(lng / 10));
		}

		out.reserve(segments);
		for (unsigned i = 0; i < segments; i++)
		{
			const Radians angle = (360_deg / segments) * i;
			const auto cos = angle.cos();
			const auto sin = angle.sin();
			out.emplace_back(center.x + radius * cos, center.y + radius * sin);
		}
	}

	List<Vector2f> ShapePrimitive::circle(
		const Vector2f& center, float radius, unsigned segments)
	{
		List<Vector2f> out;
		circle(out, center, radius, segments);
		return out;
	}

	// ELLIPSE ///////////////////////////////////////////////////////////////////
	void ShapePrimitive::ellipse(List<Vector2f>& out,
		const Vector2f& center, const Vector2f& radius, unsigned segments)
	{
		if (segments == 0)
		{
			const float a = Math::Pi * radius.x * radius.y + Math::pow(radius.x + radius.y);
			const float lng = 4 * (a / (radius.x - radius.y));
			segments = Math::max(3, Math::floor_to_int(lng / 100));
		}

		out.reserve(segments);
		for (unsigned i = 0; i < segments; i++)
		{
			const Radians angle = (360_deg / segments) * i;
			const auto cos = angle.cos();
			const auto sin = angle.sin();
			out.emplace_back(center.x + radius.x * cos, center.y + radius.y * sin);
		}
	}

	List<Vector2f> ShapePrimitive::ellipse(
		const Vector2f& center, const Vector2f& radius, unsigned segments)
	{
		List<Vector2f> points;
		ellipse(points, center, radius, segments);
		return points;
	}

	// STAR //////////////////////////////////////////////////////////////////////
	void ShapePrimitive::star(List<Vector2f>& out,
		const Vector2f& center, unsigned count, float radius, float radius_inner)
	{
		if (count >= 2)
		{
			if (radius_inner <= 0)
				radius_inner = radius * .5f;

			const int segments = count * 2;
			const float step = 360.f / segments;
			out.reserve(segments + 2);

			for (int i = 0; i <= segments; i++)
			{
				const auto angle = -Degrees(180 + step * i);
				const float size = Math::even(i) ? radius : radius_inner;

				float sin, cos;
				angle.sin_cos(sin, cos);

				out.emplace_back(center.x + size * sin, center.y + size * cos);
			}
		}
	}

	List<Vector2f> ShapePrimitive::star(const Vector2f& center,
		unsigned count, float radius, float radius_inner)
	{
		List<Vector2f> out;
		star(out, center, count, radius, radius_inner);
		return out;
	}

	// BEZIER3 ///////////////////////////////////////////////////////////////////
	void ShapePrimitive::bezier3(List<Vector2f>& out,
		const Vector2f& p0, const Vector2f& p1, const Vector2f& p2,
		unsigned segments)
	{
		if (segments == 0)
			segments = 20;

		const float step = 1.0f / static_cast<float>(segments);

		out.push_back(Curve::bezier3(0.0f, p0, p1, p2));

		for (unsigned i = 1; i < segments; i++)
		{
			out.push_back(Curve::bezier3(
				step * static_cast<float>(i), p0, p1, p2));
		}

		out.push_back(Curve::bezier3(1.0f, p0, p1, p2));
	}

	List<Vector2f> ShapePrimitive::bezier3(
		const Vector2f& p0, const Vector2f& p1, const Vector2f& p2,
		unsigned segments)
	{
		List<Vector2f> out;
		bezier3(out, p0, p1, p2, segments);
		return out;
	}

	// BEZIER4 ///////////////////////////////////////////////////////////////////
	void ShapePrimitive::bezier4(List<Vector2f>& out,
		const Vector2f& p0, const Vector2f& p1,
		const Vector2f& p2, const Vector2f& p3, unsigned segments)
	{
		if (segments == 0)
			segments = 20;

		const float step = 1.0f / static_cast<float>(segments);

		out.push_back(Curve::bezier4(0.0f, p0, p1, p2, p3));

		for (unsigned i = 1; i < segments; i++)
		{
			out.push_back(Curve::bezier4(
				step * static_cast<float>(i), p0, p1, p2, p3));
		}

		out.push_back(Curve::bezier4(1.0f, p0, p1, p2, p3));
	}

	List<Vector2f> ShapePrimitive::bezier4(
		const Vector2f& p0, const Vector2f& p1,
		const Vector2f& p2, const Vector2f& p3, unsigned segments)
	{
		List<Vector2f> out;
		bezier4(out, p0, p1, p2, p3, segments);
		return out;
	}

	// SPLINE ////////////////////////////////////////////////////////////////////
	void ShapePrimitive::spline(List<Vector2f>& out,
		const Vector2f* points, unsigned points_count, unsigned segments)
	{
		if (segments == 0)
			segments = 20;

		const float step = 1.0f / static_cast<float>(segments);

		out.push_back(Curve::spline(0.0f, points, points_count));

		for (unsigned i = 1; i < segments; i++)
		{
			out.push_back(Curve::spline(
				step * static_cast<float>(i), points, points_count));
		}

		out.push_back(Curve::spline(1.0f, points, points_count));
	}

	List<Vector2f> ShapePrimitive::spline(
		const Vector2f* points, unsigned points_count, unsigned segments)
	{
		List<Vector2f> out;
		spline(out, points, points_count, segments);
		return out;
	}
}