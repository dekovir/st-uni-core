#pragma once
#include "unicore/Surface.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class StbSurfaceLoader : public ResourceLoaderType<Surface>
	{
		UC_OBJECT(StbSurfaceLoader, ResourceLoader)
	public:
		StbSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class StbDynamicSurfaceLoader : public ResourceLoaderType<DynamicSurface>
	{
		UC_OBJECT(StbDynamicSurfaceLoader, ResourceLoader)
	public:
		StbDynamicSurfaceLoader();

		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}
#endif