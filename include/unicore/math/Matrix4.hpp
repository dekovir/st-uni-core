#pragma once
#include "unicore/math/Vector3.hpp"

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

		// TODO: Replace with span
		constexpr Matrix4(const T* m)
			: m00(m[0]), m01(m[1]), m02(m[2]), m03(m[3])
			, m10(m[4]), m11(m[5]), m12(m[6]), m13(m[7])
			, m20(m[8]), m21(m[9]), m22(m[10]), m23(m[11])
			, m30(m[12]), m31(m[13]), m32(m[14]), m33(m[15])
		{}

		constexpr Matrix4(const Matrix4& other) = default;
		constexpr Matrix4(Matrix4&& other) noexcept = default;

		~Matrix4() = default;

		Matrix4& operator=(const Matrix4& other) = default;
		Matrix4& operator=(Matrix4&& other) noexcept = default;

		Matrix4& operator*=(const Matrix4& other)
		{
			Matrix4<T> result = *this * other;
			*this = result;
			return *this;
		}

		UC_NODISCARD constexpr Vector3<T> apply(const Vector3<T>& vec) const
		{
			return (*this) * vec;
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
			const auto [sin_val, cos_val] = angle.sin_cos();
			return Matrix4(
				1, 0, 0, 0,
				0, +cos_val, +sin_val, 0,
				0, -sin_val, +cos_val, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4 rotate_y(Radians angle)
		{
			const auto [sin_val, cos_val] = angle.sin_cos();
			return Matrix4(
				+cos_val, 0, -sin_val, 0,
				0, 1, 0, 0,
				sin_val, 0, +cos_val, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4 rotate_z(Radians angle)
		{
			const auto [sin_val, cos_val] = angle.sin_cos();
			return Matrix4(
				+cos_val, +sin_val, 0, 0,
				-sin_val, +cos_val, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4 rotate(Radians angle, const Vector3<T>& axis)
		{
			const auto [sin_val, cos_val] = angle.sin_cos();
			const auto invert = 1 - cos_val;

			const T xx = axis.x * axis.x;
			const T xy = axis.x * axis.y;
			const T xz = axis.x * axis.z;
			const T yy = axis.y * axis.y;
			const T yz = axis.y * axis.z;
			const T zz = axis.z * axis.z;

			return Matrix4(
				cos_val + xx * invert, xy * invert + axis.z * cos_val, xz * invert - axis.y() * cos_val, 0,
				xy * invert - axis.z * cos_val, cos_val + yy * invert, yz * invert + axis.x() * cos_val, 0,
				xz * invert + axis.y * cos_val, yz * invert - axis.x * cos_val, cos_val + zz * invert, 0,
				0, 0, 0, 1
			);
		}

		static constexpr Matrix4 orthographic(const Vector2<T>& size, T near, T far)
		{
			const Vector2<T> xy = T(2.0) / size;
			const auto z = T(2.0) / (near - far);

			return Matrix4(
				xy.x, 0, 0, 0,
				0, xy.y, 0, 0,
				0, 0, z, 0,
				0, 0, near * z - 1, 1
			);
		}

		static constexpr Matrix4 orthographic(const Rect<T>& rect, T near, T far)
		{
			const Vector2<T> dif = { rect.top_right() - rect.bottom_left(), near - far, 1 };
			const Vector3<T> scale = T(2.0) / dif;
			const Vector3<T> offset = Vector3<T>{ rect.bottom_right() + rect.bottom_left(), near + far, 1 } / dif;

			return Matrix4(
				scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
				0, 0, scale.z, 0,
				-offset.x, -offset.y, offset.z, 1
			);
		}
	};

	using Matrix4f = Matrix4<float>;

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
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
	static constexpr Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs)
	{
		return Matrix4<T>(
			lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30,
			lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31,
			lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32,
			lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33,
			lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30,
			lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31,
			lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32,
			lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33,
			lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30,
			lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31,
			lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32,
			lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33,
			lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30,
			lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31,
			lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32,
			lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33
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
	namespace details
	{
		template<typename T>
		struct MatrixConst4
		{
			static constexpr Matrix4<T> Zero = Matrix4<T>(
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
				);

			static constexpr Matrix4<T> Identity = Matrix4<T>(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
				);
		};
	}

	using MatrixConst4f = details::MatrixConst4<float>;
}