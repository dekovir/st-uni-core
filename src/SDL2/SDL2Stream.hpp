#pragma once
#include "unicore/Stream.hpp"
#if defined(UNICORE_USE_SDL2)
#include <SDL.h>

namespace unicore
{
	class SDL2Stream : public ReadStream, public WriteStream
	{
	public:
		explicit SDL2Stream(SDL_RWops* context);
		~SDL2Stream() override;

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD SDL_RWops* get_context() { return _context; }
		UC_NODISCARD const SDL_RWops* get_context() const { return _context; }

		UC_NODISCARD bool eof() const override;
		size_t read(void* buffer, size_t size, size_t count) override;

		size_t write(const void* buffer, size_t size, size_t count) override;
		bool flush() override { return true; }

	protected:
		SDL_RWops* _context;
	};
}

#endif
