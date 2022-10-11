#pragma once
#include "unicore/Math.hpp"
#include "unicore/Hash.hpp"

namespace unicore
{
	template<typename T>
	struct Vector3
	{
		T x;
		T y;
		T z;

		constexpr Vector3() noexcept = default;
		constexpr Vector3(T x, T y) noexcept;
		constexpr Vector3(T x, T y, T z) noexcept;
		explicit constexpr Vector3(T value) noexcept;

		constexpr Vector3(const Vector3<T>& other) noexcept = default;
		constexpr Vector3(Vector3<T>&& other) noexcept = default;

		~Vector3() = default;

		void set(T x_, T y_, T z_);

		UC_NODISCARD constexpr size_t size() const { return 3; }
		UC_NODISCARD constexpr T volume() const { return x * y * z; }

		UC_NODISCARD constexpr T length_squared() const { return x * x + y * y + z * z; }
		UC_NODISCARD float length() const { return sqrtf(static_cast<float>(length_squared())); }

		T& operator[](int index) { return (&x)[index]; }
		T operator[](int index) const { return (&x)[index]; }

		Vector3& operator=(const Vector3<T>& other) = default;
		Vector3& operator=(Vector3<T>&& other) noexcept = default;

		Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		Vector3& operator*=(T value)
		{
			x += value;
			y += value;
			z += value;
			return *this;
		}

		UC_NODISCARD constexpr T min_value() const { return Math::min(x, y, z); }
		UC_NODISCARD constexpr T max_value() const { return Math::max(x, y, z); }

		void normalize()
		{
			const auto lng = length();
			x /= lng;
			y /= lng;
			z /= lng;
		}

		UC_NODISCARD Vector3<T> normalized() const
		{
			Vector3<T> vec(x, y, z);
			vec.normalize();
			return vec;
		}

		template<typename U>
		constexpr Vector3<U> cast() const
		{
			if constexpr (std::is_same_v<U, T>) return *this;

			return Vector3<U>(
				static_cast<U>(x),
				static_cast<U>(y),
				static_cast<U>(z));
		}

		UC_NODISCARD Vector3<T> snap(const Vector3<T>& value) const
		{
			return {
				Math::snap(x, value.x),
				Math::snap(y, value.y),
				Math::snap(z, value.z)
			};
		}

		static constexpr Vector3<T> lerp(const Vector3<T>& a, const Vector3<T>& b, float t)
		{
			return Vector3<T>(
				Math::lerp(a.x, b.x, t),
				Math::lerp(a.y, b.y, t),
				Math::lerp(a.z, b.z, t)
				);
		}

		static constexpr Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
		{
			return Vector3<T>(
				a[1] * b[2] - b[1] * a[2],
				a[2] * b[0] - b[2] * a[0],
				a[0] * b[1] - b[0] * a[1]
				);
		}
	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<int>   Vector3i;

	static_assert(sizeof(Vector3f) == sizeof(float) * 3);

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template <typename T>
	constexpr Vector3<T>::Vector3(T x_, T y_) noexcept
		: x(x_), y(y_), z(0)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T _x, T _y, T _z) noexcept
		: x(_x), y(_y), z(_z)
	{
	}

	template <typename T>
	constexpr Vector3<T>::Vector3(T value) noexcept
		: x(value), y(value), z(value)
	{
	}

	template <typename T>
	void Vector3<T>::set(T x_, T y_, T z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static std::enable_if_t<std::is_integral_v<T>, bool>
		operator == (const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	template<typename T>
	static std::enable_if_t<std::is_integral_v<T>, bool>
		operator != (const Vector3<T>& a, const Vector3<T>& b)
	{
		return a.x != b.x || a.y != b.y || a.z != b.z;
	}

	template<typename T>
	static Vector3<T> operator + (const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	template<typename T>
	static Vector3<T> operator - (const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	template<typename T>
	static Vector3<T> operator * (const Vector3<T>& a, const Vector3<T>& b)
	{
		return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		struct VectorConst3
		{
			static constexpr Vector3<T> Zero = Vector2<T>(0, 0, 0);
			static constexpr Vector3<T> One = Vector2<T>(1, 1, 1);

			static constexpr Vector3<T> AxisX = Vector2<T>(1, 0, 0);
			static constexpr Vector3<T> AxisY = Vector2<T>(0, 1, 0);
			static constexpr Vector3<T> AxisZ = Vector2<T>(0, 0, 1);
		};
	}

	using VectorConst3i = details::VectorConst3<int>;
	using VectorConst3f = details::VectorConst3<float>;

	UNICORE_MAKE_HASH(Vector3i)
	{
		return make(value.x, value.y, value.z);
	}

	UNICORE_MAKE_HASH(Vector3f)
	{
		return make(value.x, value.y, value.z);
	}
}