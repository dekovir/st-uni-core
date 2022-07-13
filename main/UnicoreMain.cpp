#include "UnicoreMain.h"
#if defined(UNICORE_USE_SDL2_MAIN)
#	include <SDL_main.h>
#endif

namespace unicore
{
	class State
	{
	public:
		Unique<Platform> platform;
		Unique<Renderer2D> render;
		Unique<ProxyLogger> render_logger;
		Unique<Core> core;

		State()
		{
			platform = Platform::create();

			render_logger = make_unique<ProxyLogger>("[Render] ", platform->logger);
			render = Renderer2D::create(*render_logger);

			core = create_main_core({ *platform, *render });
		}
	};

	static State* g_state = nullptr;

	void state_init()
	{
		g_state = new State();
	}

	void state_done()
	{
		delete g_state;
	}

	bool state_running()
	{
		return g_state->platform->running();
	}

	void state_frame()
	{
		g_state->core->frame();
	}
}

int main(int argc, char* argv[])
{
	unicore::state_init();

#if defined(UNICORE_PLATFORM_EMSCRIPTEN)
	emscripten_set_main_loop(unicore::state_frame, 0, 1);
#else
	while (unicore::state_running())
		unicore::state_frame();
#endif

	unicore::state_done();
	return 0;
}