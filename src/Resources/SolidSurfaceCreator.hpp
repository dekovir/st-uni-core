#pragma once
#include "unicore/ResourceCreator.hpp"
#include "unicore/Surface.hpp"

namespace unicore
{
	class SurfaceSolidSurfaceCreator
		: public ResourceCreatorData<Surface, CreateResource::SolidColor>
	{
	public:
		Shared<Surface> create_from_data(
			const ResourceCreatorContext& context,
			const CreateResource::SolidColor& data) override;
	};

	class DynamicSurfaceSolidSurfaceCreator
		: public ResourceCreatorData<DynamicSurface, CreateResource::SolidColor>
	{
	public:
		Shared<DynamicSurface> create_from_data(
			const ResourceCreatorContext& context,
			const CreateResource::SolidColor& data) override;
	};
}