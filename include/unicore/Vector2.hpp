#pragma once
#include "unicore/Math.hpp"

namespace unicore
{
	template<typename T>
	struct Vector2
	{
		T x;
		T y;

		constexpr Vector2() = default;
		constexpr Vector2(T x, T y);
		explicit constexpr Vector2(T value);

		// Copy constructor
		constexpr Vector2(const Vector2<T>& other) = default;

		// Move constructor
		constexpr Vector2(Vector2<T>&& other) noexcept
			: x(other.x), y(other.y)
		{}

		~Vector2() = default;

		void set(T _x, T _y)
		{
			x = _x;
			y = _y;
		}

		UC_NODISCARD constexpr T area() const { return x * y; }
		UC_NODISCARD constexpr T length_squared() const { return x * x + y * y; }
		UC_NODISCARD float length() const { return sqrtf(length_squared()); }

		// Copy assignment operator
		Vector2& operator=(const Vector2& other) noexcept = default;

		// Move assignment operator
		Vector2& operator=(Vector2&& other) noexcept
		{
			x = other.x;
			y = other.y;
			return  *this;
		}

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

		void normalize()
		{
			const auto lng = length();
			x /= lng;
			y /= lng;
		}

		UC_NODISCARD Vector2<T> normalized() const
		{
			Vector2<T> vec(x, y);
			vec.normalize();
			return vec;
		}

		template<typename U>
		UC_NODISCARD constexpr Vector2<U> cast() const
		{
			return Vector2<U>(static_cast<U>(x), static_cast<U>(y));
		}

		UC_NODISCARD float dot(const Vector2<T>& other) const
		{
			return x * other.x + y * other.y;
		}

		UC_NODISCARD Radians angle(const Vector2<T>& other) const
		{
			return Radians(Math::sqrt(dot(other) / (length() * other.length())));
		}

		UC_NODISCARD float distance(const Vector2<T>& other) const
		{
			return (*this - other).length();
		}

		static float dot(const Vector2<T>& a, const Vector2<T>& b)
		{
			return a.dot(b);
		}

		static Radians angle(const Vector2<T>& a, const Vector2<T>& b)
		{
			return a.angle(b);
		}

		static float distance(const Vector2<T>& a, const Vector2<T>& b)
		{
			return a.distance(b);
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