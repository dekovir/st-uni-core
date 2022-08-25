#pragma once
#include "unicore/ResourceCreator.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

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

	class DynamicTextureSizeCreator
		: public ResourceCreatorData<DynamicTexture, CreateResource::Size>
	{
	public:
		explicit DynamicTextureSizeCreator(Renderer& renderer);

	protected:
		Renderer& _renderer;

		Shared<DynamicTexture> create_from_data(
			const ResourceCreatorContext& context,
			const CreateResource::Size& data) override;
	};

	class TargetTextureSizeCreator
		: public ResourceCreatorData<TargetTexture, CreateResource::Size>
	{
	public:
		explicit TargetTextureSizeCreator(Renderer& renderer);

	protected:
		Renderer& _renderer;

		Shared<TargetTexture> create_from_data(
			const ResourceCreatorContext& context,
			const CreateResource::Size& data) override;
	};
}