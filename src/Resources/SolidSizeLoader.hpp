#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	// SurfaceSizeSurfaceLoader ///////////////////////////////////////////////////
	class SurfaceSizeSurfaceLoader
		: public ResourceLoaderOptionsTyped<Surface, SolidSizeOptions, ResourceLoaderPathPolicy::Empty>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// DynamicSurfaceSolidSizeLoader //////////////////////////////////////////////
	class DynamicSurfaceSolidSizeLoader
		: public ResourceLoaderOptionsTyped<DynamicSurface, SolidSizeOptions, ResourceLoaderPathPolicy::Empty>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// TextureSolidSizeLoader /////////////////////////////////////////////////////
	class TextureSolidSizeLoader
		: public ResourceLoaderOptionsTyped<Texture, SolidSizeOptions, ResourceLoaderPathPolicy::Empty>
	{
	public:
		explicit TextureSolidSizeLoader(Renderer& renderer);

		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;

	protected:
		Renderer& _renderer;
	};
}