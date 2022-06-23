#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "SDL2Render.hpp"
#include "SDL2Input.hpp"

namespace unicore
{
	class SDL2Timer : public Time
	{
	public:
		SDL2Timer();

		const TimeSpan& delta() const override { return _delta; }
		const TimeSpan& elapsed() const override { return  _elapsed; }

	protected:
		Uint32 _prevTicks = 0;
		TimeSpan _elapsed = TimeSpanConst::Zero;
		TimeSpan _delta = TimeSpanConst::Zero;

		void Update();

		friend class SDL2Platform;
	};

	struct SDLPlatformSettings
	{
		SDL2RenderSettings render;
	};

	class SDL2Platform : public Platform
	{
	public:
		explicit SDL2Platform(const SDLPlatformSettings& settings);
		~SDL2Platform() override;

		bool running() const override { return _running; }

		void poll_events() override;

	protected:
		SDL2Timer _time;
		SDL2Input _input;
		ProxyLogger _render_logger;
		SDL2Render _render;

		bool _running = true;

		void register_module(Context& context) override;
	};
}
#endif