#include "UnicoreMain.h"

namespace unicore
{
	class State
	{
	public:
		Shared<Platform> platform;
		Shared<Render2D> render;
		Shared<ProxyLogger> render_logger;
		Shared<Core> core;

		State()
		{
			platform = Platform::create();

			render_logger = make_shared<ProxyLogger>("[Render] ", platform->logger);
			render = Render2D::create(*render_logger);
			core = create_main_core({ *platform, *render });
		}

		~State()
		{
			core = nullptr;
		}
	};

	static State* g_state = nullptr;

	void state_frame()
	{
		g_state->core->frame();
	}
}

#if defined(UNICORE_USE_SDL2_MAIN)
#include <SDL_main.h>
#endif

int main(int argc, char* argv[])
{
	unicore::g_state = new unicore::State();

#if defined(UNICORE_PLATFORM_EMSCRIPTEN)
	emscripten_set_main_loop(unicore::state_frame, 0, 1);
#else
	while (unicore::g_state->platform->running())
		unicore::state_frame();
#endif

	delete unicore::g_state;
	return 0;
}