#pragma once
#include "unicore/Surface.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class StbSurfaceLoader : public ResourceLoaderT<Surface>
	{
		UC_OBJECT(StbSurfaceLoader, ResourceLoader)
	public:
		StbSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};

	class StbDynamicSurfaceLoader : public ResourceLoaderT<DynamicSurface>
	{
		UC_OBJECT(StbDynamicSurfaceLoader, ResourceLoader)
	public:
		StbDynamicSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};
}
#endif