#pragma once
#include "unicore/Surface.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class SurfaceLoader : public ResourceLoaderT<Surface>
	{
	public:
		SurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	class DynamicSurfaceLoader : public ResourceLoaderT<DynamicSurface>
	{
	public:
		DynamicSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}