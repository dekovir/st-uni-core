#pragma once
#include "unicore/system/Object.hpp"
#include "unicore/resource/DataModel.hpp"
#include "unicore/math/Ray.hpp"

namespace unicore
{
	using RaycastModel = DataModel2<bool>;

	class RaycastRenderer : public Object
	{
	public:
		struct Options
		{
			float max_distance = 100;
		};

		struct HitInfo
		{
			float distance;
			Vector2f point;
		};

		static bool raycast(const RaycastModel& model, const Ray2& ray, const Options& options, HitInfo& hit);
		static bool raycast(const RaycastModel& model, const Ray2& ray, HitInfo& hit);
	};
}