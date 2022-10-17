#pragma once
#include "unicore/math/Rect.hpp"

namespace unicore
{
	class ShapePrimitive
	{
	public:
		static void create_rect(List<Vector2f>& points, const Rectf& rect);
		static List<Vector2f> create_rect(const Rectf& rect);

		static void create_circle(List<Vector2f>& points, const Vector2f& center, float radius, unsigned segments = 0);
		static List<Vector2f> create_circle(const Vector2f& center, float radius, unsigned segments = 0);

		static void create_ellipse(List<Vector2f>& points, const Vector2f& center, const Vector2f& radius, unsigned segments = 0);
		static List<Vector2f> create_ellipse(const Vector2f& center, const Vector2f& radius, unsigned segments = 0);

		static void create_star(List<Vector2f>& points, const Vector2f& center, unsigned count, float radius, float radius_inner = 0);
		static List<Vector2f> create_star(const Vector2f& center, unsigned count, float radius, float radius_inner = 0);
	};
}