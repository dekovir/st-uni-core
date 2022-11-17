#pragma once
#include "unicore/math/Rect.hpp"

namespace unicore
{
	class ShapePrimitive
	{
	public:
		// RECT ////////////////////////////////////////////////////////////////////
		static void rect(List<Vector2f>& out, const Rectf& value);

		static List<Vector2f> rect(const Rectf& value);

		// CIRCLE //////////////////////////////////////////////////////////////////
		static void circle(List<Vector2f>& out,
			const Vector2f& center, float radius, unsigned segments = 0);

		static List<Vector2f> circle(
			const Vector2f& center, float radius, unsigned segments = 0);

		// ELLIPSE /////////////////////////////////////////////////////////////////
		static void ellipse(List<Vector2f>& out,
			const Vector2f& center, const Vector2f& radius, unsigned segments = 0);

		static List<Vector2f> ellipse(
			const Vector2f& center, const Vector2f& radius, unsigned segments = 0);

		// STAR ////////////////////////////////////////////////////////////////////
		static void star(List<Vector2f>& out, const Vector2f& center,
			unsigned count, float radius, float radius_inner = 0);

		static List<Vector2f> star(const Vector2f& center,
			unsigned count, float radius, float radius_inner = 0);

		// BEZIER3 /////////////////////////////////////////////////////////////////
		static void bezier3(List<Vector2f>& out,
			const Vector2f& p0, const Vector2f& p1, const Vector2f& p2,
			unsigned segments = 0);

		static List<Vector2f> bezier3(
			const Vector2f& p0, const Vector2f& p1, const Vector2f& p2,
			unsigned segments = 0);

		// BEZIER4 /////////////////////////////////////////////////////////////////
		static void bezier4(List<Vector2f>& out,
			const Vector2f& p0, const Vector2f& p1,
			const Vector2f& p2, const Vector2f& p3,
			unsigned segments = 0);

		static List<Vector2f> bezier4(
			const Vector2f& p0, const Vector2f& p1,
			const Vector2f& p2, const Vector2f& p3,
			unsigned segments = 0);

		// SPLINE //////////////////////////////////////////////////////////////////
		static void spline(List<Vector2f>& out,
			const Vector2f* points, unsigned points_count, unsigned segments = 0);

		static List<Vector2f> spline(
			const Vector2f* points, unsigned points_count, unsigned segments = 0);
	};
}