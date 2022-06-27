#include "SDL2Loaders.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/LogHelper.hpp"
#include "SDL2Utils.hpp"
#include "SDL2Render.hpp"

namespace unicore
{
	bool SDL2SurfaceLoader::can_load_extension(WStringView ext) const
	{
		return ext == L".bmp";
	}

	Shared<Resource> SDL2SurfaceLoader::load(const ResourceLoaderContext& context)
	{
		const auto c = SDL2Utils::from_stream(context.stream);
		if (SDL_Surface* surface = c != nullptr ? SDL_LoadBMP_RW(c, SDL_TRUE) : nullptr)
			return make_shared<SDL2Surface>(surface);

		UC_LOG_ERROR(context.logger) << "Failed to load surface";
		return nullptr;
	}
}
#endif