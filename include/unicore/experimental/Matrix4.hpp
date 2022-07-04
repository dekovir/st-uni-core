#pragma once
#include "unicore/Vector3.hpp"

namespace unicore
{
	template<typename T>
	struct Matrix4
	{
		T m00, m01, m02, m03;
		T m10, m11, m12, m13;
		T m20, m21, m22, m23;
		T m30, m31, m32, m33;

		constexpr Matrix4() = default;
		constexpr Matrix4(
			T m00_, T m01_, T m02_, T m03_,
			T m10_, T m11_, T m12_, T m13_,
			T m20_, T m21_, T m22_, T m23_,
			T m30_, T m31_, T m32_, T m33_)
			: m00(m00_), m01(m01_), m02(m02_), m03(m03_)
			, m10(m10_), m11(m11_), m12(m12_), m13(m13_)
			, m20(m20_), m21(m21_), m22(m22_), m23(m23_)
			, m30(m30_), m31(m31_), m32(m32_), m33(m33_)
		{}

		constexpr Matrix4(const Matrix4& other) = default;
		constexpr Matrix4(Matrix4&& other) noexcept = default;

		~Matrix4() = default;

		Matrix4& operator=(const Matrix4& other) = default;
		Matrix4& operator=(Matrix4&& other) noexcept = default;

		Matrix4& operator*(const Matrix4& other)
		{
			Matrix4<T> result = *this * other;
			*this = result;
			return *this;
		}

		static constexpr Matrix4 translate(T x, T y, T z)
		{
			return Matrix4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				x, y, z, 1
			);
		}

		static constexpr Matrix4 translate(const Vector3<T>& vec)
		{
			return translate(vec.x, vec.y, vec.z);
		}

		static constexpr Matrix4 scale(T x, T y, T z)
		{
			return Matrix4(
				x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1
			);
		}

		static constexpr Matrix4 scale(const Vector3<T>& vec)
		{
			return translate(vec.x, vec.y, vec.z);
		}

		static constexpr Matrix4 scale(T value)
		{
			return translate(value, value, value);
		}

		static Matrix4 rotate_x(Radians angle)
		{
			const auto cos_val = angle.cos();
			const auto sin_val = angle.sin();
			return Matrix4(
				1, 0, 0, 0,
				0, +cos_val, 0, +sin_val, 0,
				0, -sin_val, +cos_val, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4 rotate_y(Radians angle)
		{
			const auto cos_val = angle.cos();
			const auto sin_val = angle.sin();
			return Matrix4(
				+cos_val, 0, -sin_val, 0,
				0, 0, 0, 0,
				sin_val, 0, +cos_val, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4 rotate_z(Radians angle)
		{
			const auto cos_val = angle.cos();
			const auto sin_val = angle.sin();
			return Matrix4(
				+cos_val, +sin_val, 0, 0,
				-sin_val, +cos_val, 0, 0,
				0, 0, 0, 1
			);
		}
	};

	using Matrix4f = Matrix4<float>;

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	static constexpr bool operator==(const Matrix4<T>& a, const Matrix4<T>& b)
	{
		return
			a.m00 == b.m00 && a.m01 == b.m01 &&
			a.m10 == b.m10 && a.m11 == b.m11;
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	static constexpr bool operator!=(const Matrix4<T>& a, const Matrix4<T>& b)
	{
		return !(a == b);
	}

	template<typename T>
	static constexpr bool operator*(const Matrix4<T>& a, const Matrix4<T>& b)
	{
		return Matrix4<T>(
			a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20,
			a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21,
			a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22,
			a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20,
			a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21,
			a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22,
			a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20,
			a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21,
			a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22
			);
	}

	template<typename T>
	static constexpr Vector3<T> operator*(const Matrix4<T>& mat, const Vector3<T>& vec)
	{
		return Vector3<T>(
			mat.m00 * vec.x + mat.m01 * vec.y + mat.m02 * vec.z,
			mat.m10 * vec.x + mat.m11 * vec.y + mat.m12 * vec.z,
			mat.m20 * vec.x + mat.m21 * vec.y + mat.m22 * vec.z
			);
	}

	// CONST /////////////////////////////////////////////////////////////////////
	template<typename T>
	struct MatrixConst4
	{
		constexpr Matrix4<T> Zero = Matrix4<T>(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
			);

		constexpr Matrix4<T> Identity = Matrix4<T>(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
	};

	using MatrixConst4f = MatrixConst4<float>;
}