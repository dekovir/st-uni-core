#pragma once
#include "unicore/Surface.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct StbSurfaceLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		StbSurfaceLoadPolicy()
			: Extension({ L".png",L".tga",L".jpg",L".jpeg" })
		{
		}
	};

	class StbSurfaceLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<Surface>, StbSurfaceLoadPolicy>
	{
		UC_OBJECT(StbSurfaceLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};

	class StbDynamicSurfaceLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<DynamicSurface>, StbSurfaceLoadPolicy>
	{
		UC_OBJECT(StbDynamicSurfaceLoader, ResourceLoader)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}
#endif