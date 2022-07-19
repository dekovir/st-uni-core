#pragma once
#include "unicore/Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2Display : public Display
	{
		UC_OBJECT(SDL2Display, Display)
	public:

		explicit SDL2Display(const DisplaySettings& settings);
		~SDL2Display() override;

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD void* native_handle() const override;
		UC_NODISCARD SDL_Window* handle() const { return _handle; }

		void apply(const SDL_WindowEvent& evt);

	protected:
		SDL_Window* _handle;
		Logger& _logger;
		Vector2i _size = VectorConst2i::Zero;

		void update_size();

		static Uint32 convert_flags(DisplayFlags flags);
	};
}
#endif