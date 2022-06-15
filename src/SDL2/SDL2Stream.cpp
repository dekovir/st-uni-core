#include "SDL2Stream.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Strings.hpp"

namespace unicore
{
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

	// ===========================================================================
	bool SDL2StreamProvider::exists(const Path& path)
	{
		return false;
	}

	Optional<FileStats> SDL2StreamProvider::stats(const Path& path)
	{
		return std::nullopt;
	}

	uint16_t SDL2StreamProvider::enumerate(const Path& path, List<Path>& name_list, FileFlags flags)
	{
		return 0;
	}

	Shared<ReadStream> SDL2StreamProvider::open_read(const Path& path)
	{
		const auto utf8 = Strings::to_utf8(path.native_path());
		auto context = SDL_RWFromFile(utf8.c_str(), "r");
		return context != nullptr ? std::make_shared<SDL2Stream>(context) : nullptr;
	}

	Shared<WriteStream> SDL2StreamProvider::create_new(const Path& path)
	{
		const auto utf8 = Strings::to_utf8(path.native_path());
		auto context = SDL_RWFromFile(utf8.c_str(), "w");
		return context != nullptr ? std::make_shared<SDL2Stream>(context) : nullptr;
	}
}

#endif
