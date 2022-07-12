#pragma once
#include "unicore/Matrix2.hpp"

namespace unicore
{
	template<typename T>
	class Transform2
	{
	public:
		Vector2<T> move;
		Radians angle;
		Vector2<T> scale;

		constexpr Transform2()
			: move(VectorConst2<T>::Zero), angle(0), scale(VectorConst2<T>::One)
		{}

		explicit constexpr Transform2(const Vector2<T>& move_)
			: move(move_), angle(0), scale(VectorConst2<T>::One)
		{}

		constexpr Transform2(const Vector2<T>& move_, Radians angle_)
			: move(move_), angle(angle_), scale(VectorConst2<T>::One)
		{}

		constexpr Transform2(const Vector2<T>& move_, Radians angle_, const Vector2<T>& scale_)
			: move(move_), angle(angle_), scale(scale_)
		{}

		void clear()
		{
			move = VectorConst2<T>::Zero;
			angle = 0_rad;
			scale = VectorConst2<T>::One;
		}

		Transform2& operator*=(const Transform2& other)
		{
			const auto combined = *this * other;
			*this = combined;
			return *this;
		}

		static constexpr Transform2 moved(const Vector2<T>& move)
		{
			return Transform2(move);
		}

		static constexpr Transform2 rotated(Radians angle)
		{
			return Transform2(VectorConst2<T>::Zero, angle);
		}

		static constexpr Transform2 scaled(const Vector2<T>& scale)
		{
			return Transform2(VectorConst2<T>::Zero, 0_rad, scale);
		}
	};

	using Transform2f = Transform2<float>;

	template<typename T>
	extern constexpr Transform2<T> operator*(const Transform2<T>& a, const Transform2<T>& b)
	{
		return Transform2<T>(a * b.move, a.angle + b.angle, a.scale * b.scale);
	}

	template<typename T>
	extern constexpr Transform2<T> operator+(const Transform2<T>& a, const Transform2<T>& b)
	{
		return Transform2<T>(a.move + b.move, a.angle + b.angle, a.scale * b.scale);
	}

	template<typename T>
	extern constexpr Vector2<T> operator*(const Transform2<T>& tr, const Vector2<T>& vec)
	{
		const auto mat = Matrix2<T>::transform(tr.angle, tr.scale);
		return mat * vec + tr.move;
	}

	template<typename T>
	struct TransformConst2
	{
		static constexpr Transform2<T> Zero = Transform2<T>();
	};

	using TransformConst2f = TransformConst2<float>;
}