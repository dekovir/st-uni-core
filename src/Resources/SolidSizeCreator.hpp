#pragma once
#include "unicore/ResourceCreator.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	class SurfaceSizeSurfaceCreator
		: public ResourceCreatorData<Surface, CreateResource::SolidSize>
	{
	public:
		Shared<Surface> create_from_data(
			const Options& context,
			const CreateResource::SolidSize& data) override;
	};

	class DynamicSurfaceSolidSizeCreator
		: public ResourceCreatorData<DynamicSurface, CreateResource::SolidSize>
	{
	public:
		Shared<DynamicSurface> create_from_data(
			const Options& context,
			const CreateResource::SolidSize& data) override;
	};

	class TextureSolidSizeCreator
		: public ResourceCreatorData<Texture, CreateResource::SolidSize>
	{
	public:
		explicit TextureSolidSizeCreator(Renderer& renderer);

		Shared<Texture> create_from_data(
			const Options& context,
			const CreateResource::SolidSize& data) override;

	protected:
		Renderer& _renderer;
	};
}