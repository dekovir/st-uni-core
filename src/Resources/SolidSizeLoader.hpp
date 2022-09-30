#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	// SurfaceSizeSurfaceLoader ///////////////////////////////////////////////////
	class SurfaceSizeSurfaceLoader
		: public ResourceLoaderTypeOptions<Surface, SolidSizeOptions, ResourceLoaderPolicy::NoPath>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// DynamicSurfaceSolidSizeLoader //////////////////////////////////////////////
	class DynamicSurfaceSolidSizeLoader
		: public ResourceLoaderTypeOptions<DynamicSurface, SolidSizeOptions, ResourceLoaderPolicy::NoPath>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// TextureSolidSizeLoader /////////////////////////////////////////////////////
	class TextureSolidSizeLoader
		: public ResourceLoaderTypeOptions<Texture, SolidSizeOptions, ResourceLoaderPolicy::NoPath>
	{
	public:
		explicit TextureSolidSizeLoader(Renderer& renderer);

		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;

	protected:
		Renderer& _renderer;
	};
}