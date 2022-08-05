#pragma once
#include "unicore/Math.hpp"

namespace unicore
{
	template<typename T>
	struct Vector2
	{
		T x;
		T y;

		constexpr Vector2() noexcept = default;
		constexpr Vector2(T x, T y) noexcept;
		explicit constexpr Vector2(T value) noexcept;

		constexpr Vector2(const Vector2<T>& other) noexcept = default;
		constexpr Vector2(Vector2<T>&& other) noexcept = default;

		~Vector2() = default;

		void set(T _x, T _y)
		{
			x = _x;
			y = _y;
		}

		UC_NODISCARD constexpr size_t size() const { return 2; }
		UC_NODISCARD constexpr T area() const { return x * y; }

		UC_NODISCARD constexpr T length_squared() const { return x * x + y * y; }
		UC_NODISCARD float length() const { return sqrtf(static_cast<float>(length_squared())); }

		T& operator[](int index)
		{
			return (&x)[index];
		}

		T operator[](int index) const
		{
			return (&x)[index];
		}

		Vector2& operator=(const Vector2& other) noexcept = default;
		Vector2& operator=(Vector2&& other) noexcept = default;

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
			if constexpr (std::is_same_v<U, T>) return *this;

			return Vector2<U>(
				static_cast<U>(x),
				static_cast<U>(y));
		}

		UC_NODISCARD float dot(const Vector2<T>& other) const
		{
			return dot(*this, other);
		}

		UC_NODISCARD Vector2<T> rotate(Radians angle) const
		{
			float a_sin, a_cos;
			angle.sin_cos(a_sin, a_cos);
			return {
				x * a_cos - y * a_sin,
				x * a_sin + y * a_cos
			};
		}

		UC_NODISCARD Radians angle(const Vector2<T>& other) const
		{
			return angle(*this, other);
		}

		UC_NODISCARD float distance(const Vector2<T>& other) const
		{
			return distance(*this, other);
		}

		static float constexpr dot(const Vector2<T>& a, const Vector2<T>& b)
		{
			return a.x * b.x + a.y * b.y;
		}

		static float constexpr distance(const Vector2<T>& a, const Vector2<T>& b)
		{
			return (a - b).length();
		}

		static Radians angle(const Vector2<T>& a, const Vector2<T>& b)
		{
			return Radians(Math::acos(dot(a, b) / (a.length() * b.length())));
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int>   Vector2i;

	static_assert(sizeof(Vector2f) == sizeof(float) * 2);

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Vector2<T>::Vector2(T _x, T _y) noexcept
		: x(_x), y(_y)
	{}

	template <typename T>
	constexpr Vector2<T>::Vector2(T value) noexcept
		: x(value), y(value)
	{}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr std::enable_if_t<std::is_integral_v<T>, bool>
		operator == (const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	template<typename T>
	static constexpr std::enable_if_t<std::is_integral_v<T>, bool>
		operator != (const Vector2<T>& a, const Vector2<T>& b)
	{
		return a.x != b.x || a.y != b.y;
	}

	template<typename T>
	static constexpr Vector2<T> operator + (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x + b.x, a.y + b.y);
	}

	template<typename T>
	static constexpr Vector2<T> operator - (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x - b.x, a.y - b.y);
	}

	template<typename T>
	static constexpr Vector2<T> operator * (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x * b.x, a.y * b.y);
	}

	template<typename T>
	static constexpr std::enable_if_t<std::is_floating_point_v<T>, Vector2<T>>
		operator / (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Vector2<T>(a.x / b.x, a.y / b.y);
	}

	template<typename T>
	static constexpr Vector2<T> operator * (const Vector2<T>& a, T value)
	{
		return Vector2<T>(a.x * value, a.y * value);
	}

	template<typename T>
	static constexpr Vector2<T> operator / (const Vector2<T>& a, T value)
	{
		return Vector2<T>(a.x / value, a.y / value);
	}

	// CONST /////////////////////////////////////////////////////////////////////
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

	class LogHelper;
	template<typename T>
	extern LogHelper& operator << (LogHelper& helper, const Vector2<T>& value)
	{
		return helper << value.x << "x" << value.y;
	}
}