#pragma once
#include "unicore/Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2Display : public Display
	{
	public:
		SDL2Display(SDL_Window* handle, Logger& logger);

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD void* handle() const override;

	protected:
		SDL_Window* _handle;
		Logger& _logger;
		Vector2i _size = VectorConst2i::Zero;

		void update_size();
		friend class SDL2Platform;
	};
}
#endif