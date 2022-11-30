#pragma once
#include "unicore/math/Math.hpp"

namespace unicore
{
	namespace Curve
	{
		// BEZIER3 /////////////////////////////////////////////////////////////////
		// TODO: Make constexpr
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier3(T t,
			const TPoint& p0, const TPoint& p1, const TPoint& p2)
		{
			t = Math::clamp_01(t);

			const float t2 = t * t;
			const float mt = static_cast<T>(1) - t;
			const float mt2 = mt * mt;

			return
				p0 * mt2 +
				p1 * mt * t * static_cast<T>(2) +
				p2 * t2;
		}

		// TODO: Make constexpr
		// TODO: Replace with span
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier3(T t, const TPoint* points)
		{
			return bezier3(t, points[0], points[1], points[2]);
		}

		// BEZIER3 RATIONAL ////////////////////////////////////////////////////////
		// TODO: Make constexpr
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier3(T t,
			const TPoint& p0, const TPoint& p1, const TPoint& p2,
			const TPoint& r0, const TPoint& r1, const TPoint& r2)
		{
			t = Math::clamp_01(t);

			const float t2 = t * t;
			const float mt = static_cast<T>(1) - t;
			const float mt2 = mt * mt;

			const auto f0 = r0 * mt2;
			const auto f1 = r1 * mt * t * static_cast<T>(2);
			const auto f2 = r2 * t2;
			const auto basis = f0 + f1 + f2;

			return
				(p0 * mt2 +
					p1 * mt * t * static_cast<T>(2) +
					p2 * t2) / basis;
		}

		// TODO: Make constexpr
		// TODO: Replace with span
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier3(T t, const TPoint* points, const TPoint* ratio)
		{
			return bezier3(t,
				points[0], points[1], points[2],
				ratio[0], ratio[1], ratio[2]
			);
		}

		// BEZIER4 /////////////////////////////////////////////////////////////////
		// TODO: Make constexpr
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier4(T t,
			const TPoint& p0, const TPoint& p1, const TPoint& p2, const TPoint& p3)
		{
			t = Math::clamp_01(t);
			const float t2 = t * t;
			const float t3 = t2 * t;
			const float mt = static_cast<T>(1) - t;
			const float mt2 = mt * mt;
			const float mt3 = mt2 * mt;

			return
				p0 * mt3 +
				p1 * mt2 * t * static_cast<T>(3) +
				p2 * mt * t2 * static_cast<T>(3) +
				p3 * t3;
		}

		// TODO: Make constexpr
		// TODO: Replace with span
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier4(T t, const TPoint* points)
		{
			return bezier4(t, points[0], points[1], points[2], points[3]);
		}

		// BEZIER4 RATIONAL ////////////////////////////////////////////////////////
		// TODO: Make constexpr
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier4(T t,
			const TPoint& p0, const TPoint& p1, const TPoint& p2, const TPoint& p3,
			const TPoint& r0, const TPoint& r1, const TPoint& r2, const TPoint& r3)
		{
			t = Math::clamp_01(t);
			const float t2 = t * t;
			const float t3 = t2 * t;
			const float mt = static_cast<T>(1) - t;
			const float mt2 = mt * mt;
			const float mt3 = mt2 * mt;

			const auto f0 = r0 * mt3;
			const auto f1 = r1 * mt2 * t * static_cast<T>(3);
			const auto f2 = r2 * mt * t2 * static_cast<T>(3);
			const auto f3 = r3 * t3;

			const auto basis = f0 + f1 + f2 + f3;

			return
				(p0 * mt3 +
				p1 * mt2 * t * static_cast<T>(3) +
				p2 * mt * t2 * static_cast<T>(3) +
				p3 * t3) / basis;
		}

		// TODO: Make constexpr
		// TODO: Replace with span
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint bezier4(T t, const TPoint* points, const TPoint* ratio)
		{
			return bezier4(t,
				points[0], points[1], points[2], points[3],
				ratio[0], ratio[1], ratio[2], ratio[3]
			);
		}

		// SPLINE //////////////////////////////////////////////////////////////////
		// TODO: Make constexpr
		template<typename T, typename TPoint,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		extern TPoint spline(T t,
			const TPoint* points, unsigned points_count)
		{
			int i;
			if (t >= static_cast<T>(1))
			{
				t = static_cast<T>(1);
				i = points_count - 4;
			}
			else if (t <= static_cast<T>(0))
			{
				t = static_cast<T>(0);
				i = 0;
			}
			else
			{
				const auto curve_count = (points_count - 1) / 3;
				t *= curve_count;
				i = static_cast<int>(t);
				t -= i;
				i *= 3;
			}

			return bezier3(t, points[i + 1], points[i + 2], points[i + 3]);
		}
	};
}