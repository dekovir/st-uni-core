#include "SizeResourceCreator.hpp"

namespace unicore
{
	Shared<DynamicSurface> DynamicSurfaceSizeCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::Size& data)
	{
		return std::make_shared<DynamicSurface>(data);
	}
}