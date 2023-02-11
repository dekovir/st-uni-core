#pragma once
#include "unicore/math/Ray.hpp"

namespace unicore
{
	class IRaycastWorld
	{
	public:
		UC_NODISCARD virtual bool is_solid(const Vector2i& index) const = 0;
	};

	struct RaycastHit
	{
		Vector2i index;
		float distance;
	};

	class Raycast
	{
	public:
		using Callback = Function<Bool, const Vector2i&, float>;

		static void cast_ray(const Ray2& ray, const Callback& callback);

		static Optional<RaycastHit> cast_ray(const Ray2& ray, const IRaycastWorld& world);
	};
}