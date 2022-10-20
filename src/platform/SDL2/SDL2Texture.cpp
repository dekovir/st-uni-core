#include "SDL2Texture.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	SDL2Texture::SDL2Texture(SDL_Texture* handle)
		: SDL2BaseTexture(handle)
	{
		update_size();
	}
}
#endif