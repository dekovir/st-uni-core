#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/Surface.hpp"
#include "unicore/Texture.hpp"

namespace unicore
{
	// SurfaceSizeSurfaceLoader ///////////////////////////////////////////////////
	class SurfaceSizeSurfaceLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Multiple<Surface>,
		ResourceLoaderPathPolicy::Empty>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// DynamicSurfaceSolidSizeLoader //////////////////////////////////////////////
	class DynamicSurfaceSolidSizeLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Multiple<DynamicSurface>,
		ResourceLoaderPathPolicy::Empty>
	{
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// TextureSolidSizeLoader /////////////////////////////////////////////////////
	class TextureSolidSizeLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Multiple<Texture>,
		ResourceLoaderPathPolicy::Empty>
	{
	public:
		explicit TextureSolidSizeLoader(Renderer& renderer);

		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;

	protected:
		Renderer& _renderer;
	};
}