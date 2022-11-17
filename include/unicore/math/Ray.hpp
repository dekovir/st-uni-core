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
		TVector pos;
		TVector dir;

		constexpr Ray(const TVector& pos_, const TVector& dir_)
			: pos(pos_), dir(dir_) {}
	};

	// OPERATORS /////////////////////////////////////////////////////////////////
	template<typename TVector>
	static constexpr Bool operator==(const Ray<TVector>& a, const Ray<TVector>& b)
	{
		return
			Math::equals(a.pos, b.pos) &&
			Math::equals(a.dir, b.dir);
	}

	template<typename TVector>
	static constexpr Bool operator!=(const Ray<TVector>& a, const Ray<TVector>& b)
	{
		return !(a == b);
	}

	using Ray2 = Ray<Vector2f>;
	using Ray3 = Ray<Vector3f>;

	template<typename TVector>
	extern UNICODE_STRING_BUILDER_FORMAT(const Ray<TVector>&)
	{
		return builder << "(" << value.pos << ";" << value.dir << ")";
	}

	UNICORE_MAKE_HASH(Ray2)
	{
		return make(value.pos, value.dir);
	}

	UNICORE_MAKE_HASH(Ray3)
	{
		return make(value.pos, value.dir);
	}
}