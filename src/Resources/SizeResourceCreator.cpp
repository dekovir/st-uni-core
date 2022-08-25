#include "SizeResourceCreator.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	Shared<DynamicSurface> DynamicSurfaceSizeCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::Size& data)
	{
		return std::make_shared<DynamicSurface>(data);
	}

	// DynamicTextureSizeCreator //////////////////////////////////////////////////
	DynamicTextureSizeCreator::DynamicTextureSizeCreator(Renderer& renderer) : _renderer(renderer)
	{
	}

	Shared<DynamicTexture> DynamicTextureSizeCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::Size& data)
	{
		return _renderer.create_dynamic_texture(data);
	}

	// TargetTextureSizeCreator ///////////////////////////////////////////////////
	TargetTextureSizeCreator::TargetTextureSizeCreator(Renderer& renderer) : _renderer(renderer)
	{
	}

	Shared<TargetTexture> TargetTextureSizeCreator::create_from_data(
		const ResourceCreatorContext& context,
		const CreateResource::Size& data)
	{
		return _renderer.create_target_texture(data);
	}
}