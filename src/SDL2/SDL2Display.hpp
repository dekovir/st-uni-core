#pragma once
#include "unicore/Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	enum class SDL2DisplayFlag
	{
		Resizable = 1 << 0,
		Borderless = 1 << 1,
		Fullscreen = 1 << 2,
	};
	UNICORE_ENUMFLAGS(SDL2DisplayFlag, SDL2DisplayFlags);

	class SDL2Display : public Display
	{
	public:
		struct Settings
		{
			Logger& logger;
			StringView title;
			Vector2i size;
			SDL2DisplayFlags flags;
		};

		SDL2Display(SDL_Window* handle, Logger& logger);

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD void* handle() const override;

		void apply(const SDL_WindowEvent& evt);

		static Shared<SDL2Display> create(const Settings& settings);

	protected:
		SDL_Window* _handle;
		Logger& _logger;
		Vector2i _size = VectorConst2i::Zero;

		void update_size();

		static Uint32 convert_flags(SDL2DisplayFlags flags);
	};
}
#endif