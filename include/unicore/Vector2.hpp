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
		constexpr Vector2(T value);

		void set(T _x, T _y)
		{
			x = _x;
			y = _y;
		}

		UC_NODISCARD constexpr T area() const { return x * y; }

		Vector2<T>& operator += (const Vector2<T>& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		Vector2<T>& operator -= (const Vector2<T>& other)
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		Vector2<T>& operator *= (T value)
		{
			x *= value;
			y *= value;
			return *this;
		}

		template<typename U>
		UC_NODISCARD constexpr Vector2<U> cast() const
		{
			return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int>   Vector2i;

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Vector2<T>::Vector2(T _x, T _y)
		: x(_x), y(_y)
	{}

	template <typename T>
	constexpr Vector2<T>::Vector2(T value)
		: x(value), y(value)
	{}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static std::enable_if_t<std::is_integral_v<T>, bool>
		operator == (const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	static std::enable_if_t<std::is_integral_v<T>, bool>
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
	static std::enable_if_t<std::is_floating_point_v<T>, Vector2<T>>
		operator / (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x / b.x, a.y / b.y);
	}

	template<typename T>
	static Vector2<T> operator * (const Vector2<T>& a, T value)
	{
		return Vector2<T>(a.x * value, a.y * value);
	}

	template<typename T>
	static std::enable_if_t<std::is_floating_point_v<T>, Vector2<T>>
		operator / (const Vector2<T>& a, T value)
	{
		return Vector2<T>(a.x / value, a.y / value);
	}

	template<typename T>
	struct VectorConst2
	{
		static constexpr Vector2<T> Zero = Vector2<T>(0, 0);
		static constexpr Vector2<T> One = Vector2<T>(1, 1);

		static constexpr Vector2<T> AxisX = Vector2<T>(1, 0);
		static constexpr Vector2<T> AxisY = Vector2<T>(0, 1);
	};

	using VectorConst2i = VectorConst2<int>;
	using VectorConst2f = VectorConst2<float>;
}