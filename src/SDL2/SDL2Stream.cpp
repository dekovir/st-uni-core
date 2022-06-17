#include "SDL2Stream.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Strings.hpp"

namespace unicore
{
	SDL2Stream::SDL2Stream(SDL_RWops* context)
		: _context(context)
	{
	}

	SDL2Stream::~SDL2Stream()
	{
		SDL_RWclose(_context);
	}

	int64_t SDL2Stream::size() const
	{
		return SDL_RWsize(_context);
	}

	int64_t SDL2Stream::seek(int64_t offset, SeekMethod method)
	{
		return SDL_RWseek(_context, offset, (int)method);
	}

	bool SDL2Stream::eof() const
	{
		const auto position = SDL_RWtell(_context);
		const auto size = SDL_RWsize(_context);
		return position == size;
	}

	size_t SDL2Stream::read(void* buffer, size_t size, size_t count)
	{
		return SDL_RWread(_context, buffer, size, count);
	}

	size_t SDL2Stream::write(const void* buffer, size_t size, size_t count)
	{
		return SDL_RWwrite(_context, buffer, size, count);
	}
}

#endif