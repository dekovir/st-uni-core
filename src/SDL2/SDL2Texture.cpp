#include "SDL2Texture.hpp"
#if defined(UNICORE_USE_SDL2)

namespace unicore
{
	SDL2Texture::SDL2Texture(SDL_Texture* context)
		: _context(context)
	{
		update_size();
	}

	uint32_t SDL2Texture::video_memory_use() const
	{
		Uint32 format;
		if (SDL_QueryTexture(_context, &format, nullptr, nullptr, nullptr) == 0)
		{
			const auto bpp = SDL_BYTESPERPIXEL(format);
			return _size.area() * bpp;
		}

		return 0;
	}

	void SDL2Texture::update_size()
	{
		SDL_QueryTexture(_context, nullptr, nullptr, &_size.x, &_size.y);
	}
}
#endif