#pragma once
#include "unicore/Surface.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class STBSurfaceLoader : public ResourceLoaderT<Surface>
	{
	public:
		STBSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};

	class STBDynamicSurfaceLoader : public ResourceLoaderT<DynamicSurface>
	{
	public:
		STBDynamicSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};
}