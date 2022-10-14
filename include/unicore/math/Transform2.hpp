#pragma once
#include "unicore/math/Matrix2.hpp"

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
			: move(details::VectorConst2<T>::Zero)
			, angle(0)
			, scale(details::VectorConst2<T>::One)
		{}

		explicit constexpr Transform2(const Vector2f& move_)
			: move(move_), angle(0), scale(details::VectorConst2<T>::One)
		{}

		constexpr Transform2(const Vector2<T>& move_, Radians angle_)
			: move(move_), angle(angle_), scale(details::VectorConst2<T>::One)
		{}

		constexpr Transform2(const Vector2<T>& move_, Radians angle_, const Vector2<T>& scale_)
			: move(move_), angle(angle_), scale(scale_)
		{}

		void clear()
		{
			move = details::VectorConst2<T>::Zero;
			angle = 0_rad;
			scale = details::VectorConst2<T>::One;
		}

		Transform2& operator*=(const Transform2& other)
		{
			const auto combined = *this * other;
			*this = combined;
			return *this;
		}

		void apply(Vector2<T>& vec) const
		{
			const auto mat = Matrix2<T>::transform(angle, scale);
			vec = mat * vec + move;
		}

		static constexpr Transform2 moved(const Vector2<T>& move)
		{
			return Transform2(move);
		}

		static constexpr Transform2 rotated(Radians angle)
		{
			return{ details::VectorConst2<T>::Zero, angle };
		}

		static constexpr Transform2 scaled(const Vector2<T>& scale)
		{
			return { details::VectorConst2<T>::Zero, 0_rad, scale };
		}

		static constexpr Transform2 scaled(T scale)
		{
			return { details::VectorConst2<T>::Zero, 0_rad, Vector2<T>(scale) };
		}
	};

	using Transform2f = Transform2<Float>;

	// OPERATORS ///////////////////////////////////////////////////////////////
	template<typename T>
	extern constexpr Vector2<T> operator*(const Transform2<T>& tr, const Vector2<T>& vec)
	{
		const auto mat = Matrix2<T>::transform(tr.angle, tr.scale);
		return mat * vec + tr.move;
	}

	template<typename T>
	extern constexpr Transform2<T> operator*(const Transform2<T>& a, const Transform2<T>& b)
	{
		return { a * b.move, a.angle + b.angle, a.scale * b.scale };
	}

	template<typename T>
	extern constexpr Transform2<T> operator+(const Transform2<T>& a, const Transform2<T>& b)
	{
		return { a.move + b.move, a.angle + b.angle, a.scale * b.scale };
	}

	// CONST /////////////////////////////////////////////////////////////////////
	namespace details
	{
		template<typename T>
		struct TransformConst2
		{
			static constexpr Transform2<T> Zero = Transform2<T>();
		};
	}

	using TransformConst2f = details::TransformConst2<Float>;
}