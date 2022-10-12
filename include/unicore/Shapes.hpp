#pragma once
#include "unicore/math/Rect.hpp"

namespace unicore::Shapes
{
	extern void gen_rect(List<Vector2f>& points, const Rectf& rect);
	extern List<Vector2f> gen_rect(const Rectf& rect);

	extern void gen_circle(List<Vector2f>& points, const Vector2f& center, float radius, unsigned segments = 0);
	extern List<Vector2f> gen_circle(const Vector2f& center, float radius, unsigned segments = 0);

	extern void gen_ellipse(List<Vector2f>& points, const Vector2f& center, const Vector2f& radius, unsigned segments = 0);
	extern List<Vector2f> gen_ellipse(const Vector2f& center, const Vector2f& radius, unsigned segments = 0);

	extern void gen_star(List<Vector2f>& points, const Vector2f& center, unsigned count, float radius, float radius_inner = 0);
	extern List<Vector2f> gen_star(const Vector2f& center, unsigned count, float radius, float radius_inner = 0);
}