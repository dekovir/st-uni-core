#include "SDL2Loaders.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/LogHelper.hpp"
#include "SDL2Utils.hpp"
#include "SDL2Stream.hpp"
#include "SDL2Render.hpp"

namespace unicore
{
	bool SDL2SurfaceLoader::can_load_extension(WStringView ext) const
	{
		return ext == L".bmp";
	}

	Shared<Resource> SDL2SurfaceLoader::load(const ResourceLoaderContext& context)
	{
		SDL_Surface* surface;

		if (const auto sdl_stream = dynamic_cast<SDL2Stream*>(&context.stream))
		{
			surface = SDL_LoadBMP_RW(sdl_stream->get_context(), SDL_FALSE);
		}
		else
		{
			const auto c = SDL2Utils::from_stream(context.stream);
			surface = c != nullptr ? SDL_LoadBMP_RW(c, SDL_TRUE) : nullptr;
		}

		if (surface)
			return make_shared<SDL2Surface>(surface);

		UC_LOG_ERROR(context.logger) << "Failed to load surface";
		return nullptr;
	}
}
#endif