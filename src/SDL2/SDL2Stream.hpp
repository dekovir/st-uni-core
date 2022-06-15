#pragma once
#include "unicore/StreamProvider.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Stream : public ReadStream, public WriteStream
	{
	public:
		explicit SDL2Stream(SDL_RWops* context) : _context(context) {}
		~SDL2Stream() override;

		bool can_seek() const override { return true; }

		int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		[[nodiscard]] SDL_RWops* get_context() { return _context; }
		[[nodiscard]] const SDL_RWops* get_context() const { return _context; }

		bool eof() const override;
		size_t read(void* buffer, size_t size, size_t count) override;

		size_t write(const void* buffer, size_t size, size_t count) override;
		bool flush() override { return true; }

	protected:
		SDL_RWops* _context;
	};

	class SDL2StreamProvider : public StreamProvider
	{
	public:
		bool exists(const Path& path) override;
		Optional<FileStats> stats(const Path& path) override;

		uint16_t enumerate(const Path& path, List<Path>& name_list, FileFlags flags) override;

		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;
	};
}

#endif
