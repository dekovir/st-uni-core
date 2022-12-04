#pragma once
#include "unicore/math/Ray.hpp"

namespace unicore
{
	class Raycast
	{
	public:
		using Callback = Function<Bool, const Vector2i&, float>;

		static void cast_ray(const Ray2& ray, const Callback& callback);
	};
}