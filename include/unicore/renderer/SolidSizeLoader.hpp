#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/renderer/Surface.hpp"
#include "unicore/renderer/Texture.hpp"

namespace unicore
{
	// SurfaceSizeSurfaceLoader ///////////////////////////////////////////////////
	class SurfaceSizeSurfaceLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Single<Surface>,
		ResourceLoaderPathPolicy::Empty>
	{
		UC_OBJECT(SurfaceSizeSurfaceLoader, ResourceLoader)
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// DynamicSurfaceSolidSizeLoader //////////////////////////////////////////////
	class DynamicSurfaceSolidSizeLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Single<DynamicSurface>,
		ResourceLoaderPathPolicy::Empty>
	{
		UC_OBJECT(DynamicSurfaceSolidSizeLoader, ResourceLoader)
	public:
		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;
	};

	// TextureSolidSizeLoader /////////////////////////////////////////////////////
	class TextureSolidSizeLoader : public ResourceLoaderOptionsTyped<
		SolidSizeOptions,
		ResourceLoaderTypePolicy::Single<Texture>,
		ResourceLoaderPathPolicy::Empty>
	{
		UC_OBJECT(TextureSolidSizeLoader, ResourceLoader)
	public:
		explicit TextureSolidSizeLoader(Renderer& renderer);

		Shared<Resource> load_options(const Context& context,
			const SolidSizeOptions& options) override;

	protected:
		Renderer& _renderer;
	};
}