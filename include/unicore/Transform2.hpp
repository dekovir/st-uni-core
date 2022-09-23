#pragma once
#include "unicore/Matrix2.hpp"

namespace unicore
{
	class Transform2
	{
	public:
		Vector2f move;
		Radians angle;
		Vector2f scale;

		constexpr Transform2()
			: move(VectorConst2f::Zero), angle(0), scale(VectorConst2f::One)
		{}

		explicit constexpr Transform2(const Vector2f& move_)
			: move(move_), angle(0), scale(VectorConst2f::One)
		{}

		constexpr Transform2(const Vector2f& move_, Radians angle_)
			: move(move_), angle(angle_), scale(VectorConst2f::One)
		{}

		constexpr Transform2(const Vector2f& move_, Radians angle_, const Vector2f& scale_)
			: move(move_), angle(angle_), scale(scale_)
		{}

		void clear()
		{
			move = VectorConst2f::Zero;
			angle = 0_rad;
			scale = VectorConst2f::One;
		}

		Transform2& operator*=(const Transform2& other);

		static constexpr Transform2 moved(const Vector2f& move)
		{
			return Transform2(move);
		}

		static constexpr Transform2 rotated(Radians angle)
		{
			return{ VectorConst2f::Zero, angle };
		}

		static constexpr Transform2 scaled(const Vector2f& scale)
		{
			return { VectorConst2f::Zero, 0_rad, scale };
		}

		static constexpr Transform2 scaled(Float scale)
		{
			return { VectorConst2f::Zero, 0_rad, Vector2f(scale) };
		}
	};

	extern constexpr Vector2f operator*(const Transform2& tr, const Vector2f& vec)
	{
		const auto mat = Matrix2f::transform(tr.angle, tr.scale);
		return mat * vec + tr.move;
	}

	extern constexpr Transform2 operator*(const Transform2& a, const Transform2& b)
	{
		return { a * b.move, a.angle + b.angle, a.scale * b.scale };
	}

	extern constexpr Transform2 operator+(const Transform2& a, const Transform2& b)
	{
		return { a.move + b.move, a.angle + b.angle, a.scale * b.scale };
	}
}