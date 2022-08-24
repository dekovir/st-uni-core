#pragma once
#include "unicore/Surface.hpp"
#if defined(UNICORE_USE_STB)
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class STBSurfaceLoader : public ResourceLoaderT<Surface>
	{
	public:
		STBSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	class STBDynamicSurfaceLoader : public ResourceLoaderT<DynamicSurface>
	{
	public:
		STBDynamicSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}
#endif