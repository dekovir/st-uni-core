#pragma once
#include "unicore/Display.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Utils.hpp"

namespace unicore
{
	class SDL2Platform;

	class SDL2Display : public Display, public SDL2EventListener
	{
		UC_OBJECT(SDL2Display, Display)
	public:

		SDL2Display(SDL2Platform& platform, const DisplaySettings& settings);
		~SDL2Display() override;

		UC_NODISCARD const Vector2i& size() const override { return _size; }
		UC_NODISCARD DisplayMode mode() const override;
		UC_NODISCARD DisplayWindowFlags window_flags() const override;

		void set_windowed(const Vector2i& size, DisplayWindowFlags window_flags) override;
		void set_fullscreen() override;

		UC_NODISCARD Vector2i get_maximum_size() const override;
		UC_NODISCARD Vector2i get_minimum_size() const override;

		UC_NODISCARD void* native_handle() const override;
		UC_NODISCARD SDL_Window* handle() const { return _handle; }

		bool on_event(const SDL_Event& evt) override;

	protected:
		SDL2Platform& _platform;
		SDL_Window* _handle;

		Vector2i _size = VectorConst2i::Zero;

		void update_size();

		Uint32 make_flags(Vector2i& size, DisplayMode mode, DisplayWindowFlags window_flags) const;
	};
}
#endif