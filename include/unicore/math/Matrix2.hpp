#pragma once
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	template<typename T>
	struct Matrix2
	{
		Vector2<T> mat[2];

		constexpr Matrix2() = default;

		constexpr Matrix2(const Vector2<T>& x, const Vector2<T>& y)
			: mat{ x, y }
		{}

		constexpr Matrix2(
			T m00, T m01,
			T m10, T m11)
			: mat{ { m00, m01 }, { m10, m11 } }
		{}

		constexpr Matrix2(const Matrix2& other) = default;
		constexpr Matrix2(Matrix2&& other) noexcept = default;

		~Matrix2() = default;

		Matrix2& operator=(const Matrix2& other) = default;
		Matrix2& operator=(Matrix2&& other) noexcept = default;

		UC_NODISCARD constexpr size_t size() const { return 4; }

		UC_NODISCARD constexpr T determinant() const
		{
			return mat[0].x * mat[1].y - mat[0].y * mat[1].x;
		}

		Matrix2& operator*=(T value)
		{
			mat[0] *= value;
			mat[1] *= value;
			return *this;
		}

		Matrix2& operator*=(const Matrix2<T>& other)
		{
			Matrix2<T> result = *this * other;
			*this = result;
			return *this;
		}

		Matrix2& operator+=(const Matrix2<T>& other)
		{
			mat[0] += other[0];
			mat[1] += other[1];
			return *this;
		}

		Matrix2& operator-=(const Matrix2<T>& other)
		{
			mat[0] -= other[0];
			mat[1] -= other[1];
			return *this;
		}

		constexpr Vector2<T>& operator[](int index) { return mat[index]; }
		constexpr const Vector2<T>& operator[](int index) const { return mat[index]; }

		constexpr Matrix2 operator-() const
		{
			return Matrix2(-mat[0], -mat[1]);
		}

		UC_NODISCARD constexpr Matrix2 transpose() const
		{
			return Matrix2(mat[0].x, mat[1].x, mat[0].y, mat[1].y);
		}

		static constexpr Matrix2 scale(const Vector2<T>& value)
		{
			return {
				value.x, 0,
				0, value.y
			};
		}

		static constexpr Matrix2 scale(T value)
		{
			return {
				value, 0,
				0, value
			};
		}

		static constexpr Matrix2 rotation(Radians angle)
		{
			float sin = 0, cos = 0;
			angle.sin_cos(sin, cos);

			return {
				+cos, +sin,
				-sin, +cos
			};
		}

		static constexpr Matrix2 transform(Radians angle, const Vector2<T>& scale)
		{
			float sin = 0, cos = 0;
			angle.sin_cos(sin, cos);

			return Matrix2<T>(
				+cos * scale.x,
				+sin * scale.y,
				-sin * scale.x,
				+cos * scale.y);
		}
	};

	using Matrix2f = Matrix2<float>;

	static_assert(sizeof(Matrix2f) == sizeof(float) * 4);
	static_assert(std::is_polymorphic_v<Matrix2f> == false);

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	static constexpr bool operator==(const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return
			a.mat[0] == b.mat[0] &&
			a.mat[1] == b.mat[1];
	}

	template<typename T>
	static constexpr bool operator!=(const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return !(a == b);
	}

	template<typename T>
	static constexpr Matrix2<T> operator*(const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return Matrix2<T>(
			a[0].x * b[0].x + a[0].y * b[1].x,
			a[0].x * b[0].y + a[0].y * b[1].y,
			a[1].x * b[0].x + a[1].y * b[1].x,
			a[1].x * b[0].y + a[1].y * b[1].y);
	}

	template<typename T>
	static constexpr bool operator+(const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return Matrix2<T>(a[0] + b[0], a[1] + b[1]);
	}

	template<typename T>
	static constexpr bool operator-(const Matrix2<T>& a, const Matrix2<T>& b)
	{
		return Matrix2<T>(a[0] - b[0], a[1] - b[1]);
	}

	template<typename T>
	static constexpr bool operator*(const Matrix2<T>& mat, T value)
	{
		return Matrix2<T>(mat[0] * value, mat[1] * value);
	}

	template<typename T>
	static constexpr bool operator/(const Matrix2<T>& mat, T value)
	{
		return Matrix2<T>(mat[0] / value, mat[1] / value);
	}

	template<typename T>
	static constexpr Vector2<T> operator*(const Matrix2<T>& mat, const Vector2<T>& vec)
	{
		return Vector2<T>(
			mat[0].x * vec.x + mat[0].y * vec.y,
			mat[1].x * vec.x + mat[1].y * vec.y);
	}

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		struct MatrixConst2
		{
			static constexpr Matrix2<T> Zero = Matrix2<T>(
				0, 0,
				0, 0);

			static constexpr Matrix2<T> Identity = Matrix2<T>(
				1, 0,
				0, 1);
		};
	}

	using MatrixConst2f = details::MatrixConst2<float>;
}