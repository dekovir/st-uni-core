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

		UC_NODISCARD const DisplayMode& get_mode() const override { return _mode; }
		void set_mode(const DisplayMode& mode) override;

		UC_NODISCARD Vector2i get_maximum_size() const override;
		UC_NODISCARD Vector2i get_minimum_size() const override;

		UC_NODISCARD void* native_handle() const override;
		UC_NODISCARD SDL_Window* handle() const { return _handle; }

		bool on_event(const SDL_Event& evt) override;

	protected:
		SDL2Platform& _platform;
		SDL_Window* _handle;
		DisplayMode _mode;

		void update_mode();

		Uint32 make_flags(DisplayMode& mode) const;
	};
}
#endif