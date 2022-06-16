#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename T>
	struct Vector2
	{
		T x;
		T y;

		constexpr Vector2() = default;
		constexpr Vector2(T x, T y);
		constexpr Vector2(const Vector2& other)
			: x(other.x), y(other.y)
		{}

		constexpr T area() const { return x * y; }

		template<typename U>
		constexpr Vector2<U> cast() const
		{
			return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
		}

		static const Vector2<T> Zero;
		static const Vector2<T> One;
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int>   Vector2i;

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template<typename T>
	const Vector2<T> Vector2<T>::Zero{ 0, 0 };

	template<typename T>
	const Vector2<T> Vector2<T>::One{ 1, 1 };

	template <typename T>
	constexpr Vector2<T>::Vector2(T _x, T _y)
		: x(_x), y(_y)
	{
	}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value, bool>::type
		operator == (const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	static typename std::enable_if<std::is_integral<T>::value, bool>::type
		operator != (const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x != b.x || a.y != b.y;
	}

	template<typename T>
	static Vector2<T> operator + (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x + b.x, a.y + b.y);
	}

	template<typename T>
	static Vector2<T> operator - (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template<typename T>
	static Vector2<T> operator * (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x * b.x, a.y * b.y);
	}

	template<typename T>
	static Vector2<T> operator / (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x / b.x, a.y / b.y);
	}
}