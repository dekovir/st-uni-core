#pragma once
#include "unicore/Vector2.hpp"

namespace unicore
{
	template<typename T>
	struct Rect
	{
		T x, y;
		T w, h;

		constexpr Rect() = default;
		constexpr Rect(T x, T y, T w, T h);
		constexpr Rect(T x, T y, const Vector2<T>& size);
		constexpr Rect(const Vector2<T>& position, T w, T h);
		constexpr Rect(const Vector2<T>& position, const Vector2<T>& size);

		constexpr T min_x() const { return x; }
		constexpr T max_x() const { return x + w; }
		constexpr T min_y() const { return y; }
		constexpr T max_y() const { return y + h; }

		constexpr T get_left() const { return x; }
		constexpr T get_right() const { return x + w; }
		constexpr T get_bottom() const { return y; }
		constexpr T get_top() const { return y + h; }

		constexpr Vector2<T> get_position() const { return Vector2<T>(x, y); }
		constexpr Vector2<T> get_size() const { return Vector2<T>(w, h); }
		constexpr Vector2<T> get_center() const { return Vector2<T>(x + w / 2, y + h / 2); }

		constexpr Vector2<T> top_left() const { return Vector2<T>(x, y + h); }
		constexpr Vector2<T> top_right() const { return Vector2<T>(x + w, y + h); }

		constexpr Vector2<T> bottom_left() const { return Vector2<T>(x, y); }
		constexpr Vector2<T> bottom_right() const { return Vector2<T>(x + w, y); }

		constexpr bool operator== (const Rect<T>& other) const
		{
			return x == other.x && y == other.y
				&& w == other.w && h == other.h;
		}

		static constexpr Rect<T> FromMinMax(const Vector2<T>& min, const Vector2<T>& max);

		static const Rect<T> Zero;
	};

	template<typename T>
	const Rect<T> Rect<T>::Zero{ 0, 0, 0, 0 };

	typedef Rect<int>   Recti;
	typedef Rect<float> Rectf;

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Rect<T>::Rect(T _x, T _y, T _w, T _h)
		: x(_x), y(_y), w(_w), h(_h)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(T _x, T _y, const Vector2<T>& size)
		: x(_x), y(_y), w(size.x), h(size.y)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(const Vector2<T>& position, T _w, T _h)
		: x(position.x), y(position.y), w(_w), h(_h)
	{
	}

	template <typename T>
	constexpr Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size)
		: x(position.x), y(position.y), w(size.x), h(size.y)
	{
	}

	template <typename T>
	constexpr Rect<T> Rect<T>::FromMinMax(const Vector2<T>& min, const Vector2<T>& max)
	{
		return Rect<T>(min.x, min.y, max.x - min.x, max.y - min.y);
	}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value, bool>::type
		operator == (const Rect<T>& a, const Rect<T>& b)
	{
		return a.x == b.x && a.y == b.y &&
			a.w == b.w && a.h == b.h;
	}

	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value, bool>::type
		operator != (const Rect<T>& a, const Rect<T>& b)
	{
		return a.x != b.x || a.y != b.y ||
			a.w != b.w || a.h != b.h;
	}
}