#pragma once
#include "unicore/ResourceCreator.hpp"
#include "unicore/Surface.hpp"

namespace unicore
{
	class DynamicSurfaceSizeCreator
		: public ResourceCreatorData<DynamicSurface, CreateResource::Size>
	{
	protected:
		Shared<DynamicSurface> create_from_data(
			const ResourceCreatorContext& context,
			const CreateResource::Size& data) override;
	};
}