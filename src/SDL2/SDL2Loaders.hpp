#pragma once
#include "unicore/ResourceLoader.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Render2D.hpp"

namespace unicore
{
	class SDL2SurfaceLoader : public ResourceLoaderT<Surface>
	{
	public:
		UC_NODISCARD bool can_load_extension(WStringView ext) const override;
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}
#endif