#include "SDL2StreamProvider.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Strings.hpp"

namespace unicore
{
	bool SDL2StreamProvider::exists(const Path& path) const
	{
		UC_ASSERT_ALWAYS_MSG("Not implemented");
		return false;
	}

	Optional<FileStats> SDL2StreamProvider::stats(const Path& path) const
	{
		UC_ASSERT_ALWAYS_MSG("Not implemented");
		return std::nullopt;
	}

	uint16_t SDL2StreamProvider::enumerate(const Path& path,
		List<WString>& name_list, FileFlags flags)
	{
		UC_ASSERT_ALWAYS_MSG("Not implemented");
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