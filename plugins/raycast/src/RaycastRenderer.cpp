#include "unicore/raycast/RaycastRenderer.hpp"

namespace unicore
{
	bool RaycastRenderer::raycast(const RaycastModel& model, const Ray2& ray, const Options& options, HitInfo& hit)
	{
		const Vector2f unit_step = {
			Math::sqrt(1 + Math::pow(ray.direction.y / ray.direction.x)),
			Math::sqrt(1 + Math::pow(ray.direction.x / ray.direction.y))
		};

		Vector2i map_check = { Math::floor_to_int(ray.origin.x), Math::floor_to_int(ray.origin.y) };
		Vector2f ray_length;
		Vector2i step;

		if (ray.direction.x < 0)
		{
			step.x = -1;
			ray_length.x = (ray.origin.x - static_cast<float>(map_check.x)) * unit_step.x;
		}
		else
		{
			step.x = 1;
			ray_length.x = (static_cast<float>(map_check.x + 1) - ray.origin.x) * unit_step.x;
		}

		if (ray.direction.y < 0)
		{
			step.y = -1;
			ray_length.y = (ray.origin.y - static_cast<float>(map_check.y)) * unit_step.y;
		}
		else
		{
			step.y = 1;
			ray_length.y = (static_cast<float>(map_check.y + 1) - ray.origin.y) * unit_step.y;
		}

		constexpr float max_distance = 100;

		float distance = 0;

		while (distance < max_distance)
		{
			if (ray_length.x < ray_length.y)
			{
				map_check.x += step.x;
				distance = ray_length.x;
				ray_length.x += unit_step.x;
			}
			else
			{
				map_check.y += step.y;
				distance = ray_length.y;
				ray_length.y += unit_step.y;
			}

			if (model.get_at(map_check))
			{
				hit.distance = distance;
				hit.point = ray.origin + ray.direction * distance;
				return true;
			}
		}

		return false;
	}

	bool RaycastRenderer::raycast(const RaycastModel& model, const Ray2& ray, HitInfo& hit)
	{
		return raycast(model, ray, {}, hit);
	}
}