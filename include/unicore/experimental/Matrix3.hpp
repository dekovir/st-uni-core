#pragma once
#include "unicore/Vector2.hpp"
#include "unicore/Vector3.hpp"

namespace unicore
{
	template<typename T>
	struct Matrix3
	{
		Vector3<T> mat[3];

		constexpr Matrix3() = default;
		constexpr Matrix3(const Vector2<T>& x, const Vector2<T>& y, const Vector2<T>& z)
			: mat(x, y, z)
		{}

		constexpr Matrix3(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22)
			: mat{ { m00, m01, m02 }, { m10, m11, m12 }, { m20, m21, m22 } }
		{}

		constexpr Matrix3(const Matrix3& other) = default;
		constexpr Matrix3(Matrix3&& other) noexcept = default;

		~Matrix3() = default;

		Matrix3& operator=(const Matrix3& other) = default;
		Matrix3& operator=(Matrix3&& other) noexcept = default;

		UC_NODISCARD constexpr size_t size() const { return 9; }

		UC_NODISCARD constexpr T determinant() const
		{
			const float det2_12_01 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
			const float det2_12_02 = mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0];
			const float det2_12_12 = mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1];
			return mat[0][0] * det2_12_12 - mat[0][1] * det2_12_02 + mat[0][2] * det2_12_01;
		}

		Matrix3& operator*(T value)
		{
			mat[0] *= value;
			mat[1] *= value;
			mat[2] *= value;
			return *this;
		}

		Matrix3& operator*=(const Matrix3& other)
		{
			Matrix3<T> result = *this * other;
			*this = result;
			return *this;
		}

		Matrix3& operator+=(const Matrix3& other)
		{
			mat[0] += other[0];
			mat[1] += other[1];
			mat[2] += other[2];
			return *this;
		}

		Matrix3& operator-=(const Matrix3& other)
		{
			mat[0] -= other[0];
			mat[1] -= other[1];
			mat[2] -= other[2];
			return *this;
		}

		constexpr Vector3<T>& operator[](int index) { return mat[index]; }
		constexpr const Vector3<T>& operator[](int index) const { return mat[index]; }

		constexpr Matrix3 operator-() const
		{
			return Matrix3(-mat[0], -mat[1], -mat[2]);
		}

		static constexpr Matrix3 translate(T x, T y)
		{
			return Matrix3(
				1, 0, 0,
				0, 1, 0,
				x, y, 1
			);
		}

		static constexpr Matrix3 translate(const Vector2<T>& vec)
		{
			return translate(vec.x, vec.y);
		}

		static constexpr Matrix3 scale(const Vector3<T>& vec)
		{
			return Matrix3(
				vec.x, 0, 0,
				0, vec.y, 0,
				0, 0, vec.z
			);
		}

		static constexpr Matrix3 scale(T value)
		{
			return Matrix3(
				value, 0, 0,
				0, value, 0,
				0, 0, value
			);
		}

		static Matrix3 rotation(Radians angle)
		{
			const auto cos_val = angle.cos();
			const auto sin_val = angle.sin();
			return Matrix3(
				+cos_val, +sin_val, 0,
				-sin_val, +cos_val, 0,
				0, 0, 1
			);
		}
	};

	using Matrix3f = Matrix3<float>;

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	static constexpr bool operator==(const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return
			a.mat[0] == b.mat[0] &&
			a.mat[1] == b.mat[1] &&
			a.mat[2] == b.mat[2];
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	static constexpr bool operator!=(const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return !(a == b);
	}

	template<typename T>
	static constexpr bool operator*(const Matrix3<T>& a, const Matrix3<T>& b)
	{
		return Matrix3<T>(
			a.mat[0].x * b.mat[0].x + a.mat[0].y * b.mat[1].x + a.mat[0].z * b.mat[2].x,
			a.mat[0].x * b.mat[0].y + a.mat[0].y * b.mat[1].y + a.mat[0].z * b.mat[2].y,
			a.mat[0].x * b.mat[0].z + a.mat[0].y * b.mat[1].z + a.mat[0].z * b.mat[2].z,
			a.mat[1].x * b.mat[0].x + a.mat[1].y * b.mat[1].x + a.mat[1].z * b.mat[2].x,
			a.mat[1].x * b.mat[0].y + a.mat[1].y * b.mat[1].y + a.mat[1].z * b.mat[2].y,
			a.mat[1].x * b.mat[0].z + a.mat[1].y * b.mat[1].z + a.mat[1].z * b.mat[2].z,
			a.mat[2].x * b.mat[0].x + a.mat[2].y * b.mat[1].x + a.mat[2].z * b.mat[2].x,
			a.mat[2].x * b.mat[0].y + a.mat[2].y * b.mat[1].y + a.mat[2].z * b.mat[2].y,
			a.mat[2].x * b.mat[0].z + a.mat[2].y * b.mat[1].z + a.mat[2].z * b.mat[2].z
			);
	}

	template<typename T>
	static constexpr Vector3<T> operator*(const Matrix3<T>& mat, const Vector3<T>& vec)
	{
		return Vector3<T>(
			mat[0].x * vec.x + mat[1].x * vec.y + mat[2].x * vec.z,
			mat[0].y * vec.x + mat[1].y * vec.y + mat[2].y * vec.z,
			mat[0].z * vec.x + mat[1].z * vec.y + mat[2].z * vec.z);
	}

	template<typename T>
	static constexpr Vector3<T> operator*(const Matrix3<T>& mat, T value)
	{
		return Vector3<T>(mat[0] * value, mat[1] * value, mat[2] * value);
	}

	template<typename T>
	static constexpr Vector3<T> operator+(const Matrix3<T>& mat, T value)
	{
		return Vector3<T>(mat[0] + value, mat[1] + value, mat[2] + value);
	}

	template<typename T>
	static constexpr Vector3<T> operator-(const Matrix3<T>& mat, T value)
	{
		return Vector3<T>(mat[0] - value, mat[1] - value, mat[2] - value);
	}

	// CONST /////////////////////////////////////////////////////////////////////
	template<typename T>
	struct MatrixConst3
	{
		static constexpr Matrix3<T> Zero = Matrix3<T>(
			0, 0, 0,
			0, 0, 0,
			0, 0, 0
			);

		static constexpr Matrix3<T> Identity = Matrix3<T>(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);
	};

	using MatrixConst3f = MatrixConst3<float>;
}