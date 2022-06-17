#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename T>
	struct Vector3
	{
		T x;
		T y;
		T z;

		constexpr Vector3() = default;
		constexpr Vector3(T x, T y, T z);

		UC_NODISCARD constexpr T volume() const { return x * y * z; }

		template<typename U>
		constexpr Vector3<U> cast() const
		{
			return Vector3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
		}

		static const Vector3<T> Zero;
		static const Vector3<T> One;
	};

	typedef Vector3<float> Vector3f;
	typedef Vector3<int>   Vector3i;

	// IMPLEMENTATION //////////////////////////////////////////////////////////
	template<typename T>
	const Vector3<T> Vector3<T>::Zero(0, 0, 0);

	template<typename T>
	const Vector3<T> Vector3<T>::One(1, 1, 1);

	template <typename T>
	constexpr Vector3<T>::Vector3(T _x, T _y, T _z)
		: x(_x), y(_y), z(_z)
	{
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
}