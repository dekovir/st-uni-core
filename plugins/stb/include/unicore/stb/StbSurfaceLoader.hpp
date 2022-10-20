#pragma once
#include "unicore/renderer/Surface.hpp"
#if defined(UNICORE_USE_STB_IMAGE)
#include "unicore/resource/ResourceLoader.hpp"

namespace unicore
{
	struct StbSurfaceLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		StbSurfaceLoadPolicy()
			: Extension({ ".png", ".tga", ".jpg", ".jpeg" })
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