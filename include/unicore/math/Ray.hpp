#pragma once
#include "unicore/math/Vector2.hpp"
#include "unicore/math/Vector3.hpp"

namespace unicore
{
	// TODO: See implementation at  https://github.com/stride3d/stride/blob/master/sources/core/Stride.Core.Mathematics/Ray.cs
	template<typename TVector>
	class Ray
	{
	public:
		TVector origin;
		TVector direction;

		constexpr Ray(const TVector& pos_, const TVector& dir_)
			: origin(pos_), direction(dir_) {}

		UC_NODISCARD constexpr TVector point(const Float distance) const
		{
			return origin + direction * distance;
		}

		UC_NODISCARD constexpr TVector project(const TVector& point) const
		{
			return origin + direction * TVector::dot(point - origin, direction);
		}

		// TODO: Replace with constexpr
		UC_NODISCARD auto distance(const TVector& point) const
		{
			return (point - project(point)).length();
		}
	};

	// OPERATORS /////////////////////////////////////////////////////////////////
	template<typename TVector>
	static constexpr Bool operator==(const Ray<TVector>& a, const Ray<TVector>& b)
	{
		return
			Math::equals(a.origin, b.origin) &&
			Math::equals(a.direction, b.direction);
	}

	template<typename TVector>
	static constexpr Bool operator!=(const Ray<TVector>& a, const Ray<TVector>& b)
	{
		return !(a == b);
	}

	using Ray2 = Ray<Vector2f>;
	using Ray3 = Ray<Vector3f>;

	static_assert(sizeof(Ray2) == sizeof(float) * 4);
	static_assert(std::is_polymorphic_v<Ray2> == false);

	template<typename TVector>
	extern UNICODE_STRING_BUILDER_FORMAT(const Ray<TVector>&)
	{
		return builder << value.origin << "->" << value.direction;
	}

	UNICORE_MAKE_HASH(Ray2)
	{
		return make(value.origin, value.direction);
	}

	UNICORE_MAKE_HASH(Ray3)
	{
		return make(value.origin, value.direction);
	}
}