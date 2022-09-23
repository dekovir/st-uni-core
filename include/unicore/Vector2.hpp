#pragma once
#include "unicore/Math.hpp"
#include "unicore/Angle.hpp"

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

		UC_NODISCARD constexpr T min_value() const { return Math::min(x, y); }
		UC_NODISCARD constexpr T max_value() const { return Math::max(x, y); }

		UC_NODISCARD Vector2<T> normalized() const
		{
			Vector2<T> vec(x, y);
			vec.normalize();
			return vec;
		}

		UC_NODISCARD constexpr Vector2<T> perpendicular() const
		{
			//return Vector2(y, -x);
			return Vector2(-y, x);
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

		UC_NODISCARD Vector2<T> snap(const Vector2<T>& value) const
		{
			return { Math::snap(x, value.x), Math::snap(y, value.y) };
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

		static Vector2<T> reflect(const Vector2<T>& direction, const Vector2<T>& normal)
		{
			const auto factor = -2.f * dot(normal, direction);
			return Vector2<T>(factor * normal.x + direction.x, factor * normal.y + direction.y);
		}

		static constexpr Vector2<T> lerp(const Vector2<T>& a, const Vector2<T>& b, float t)
		{
			return Vector2<T>(
				Math::lerp(a.x, b.x, t),
				Math::lerp(a.y, b.y, t)
				);
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int>   Vector2i;

	static_assert(sizeof(Vector2f) == sizeof(float) * 2);

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Vector2<T>::Vector2(T x_, T y_) noexcept
		: x(x_), y(y_)
	{}

	template <typename T>
	constexpr Vector2<T>::Vector2(T value) noexcept
		: x(value), y(value)
	{}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr bool operator == (const Vector2<T>& a, const Vector2<T>& b)
	{
		return Math::equals(a.x, b.x) && Math::equals(a.y, b.y);
	}

	template<typename T>
	static constexpr bool operator != (const Vector2<T>& a, const Vector2<T>& b)
	{
		return !(a == b);
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
	static constexpr Vector2<T> operator / (const Vector2<T>& a, const Vector2<T>& b)
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
	namespace details
	{
		template<typename T>
		struct VectorConst2
		{
			static constexpr Vector2<T> Zero = Vector2<T>(0, 0);
			static constexpr Vector2<T> One = Vector2<T>(1, 1);

			static constexpr Vector2<T> AxisX = Vector2<T>(1, 0);
			static constexpr Vector2<T> AxisY = Vector2<T>(0, 1);
		};
	}

	using VectorConst2i = details::VectorConst2<int>;
	using VectorConst2f = details::VectorConst2<float>;

	template<typename T>
	extern UNICODE_STRING_BUILDER_FORMAT(const Vector2<T>&)
	{
		return builder << value.x << "x" << value.y;
	}
}

template<typename T>
struct std::hash<unicore::Vector2<T>>
{
	std::size_t operator()(const unicore::Vector2<T>& value) const noexcept
	{
		return std::hash<T>{}(value.x) ^ std::hash<T>{}(value.y << 2);
	}
};