#include "UnicoreMain.h"

namespace unicore
{
	class State
	{
	public:
		Shared<Platform> platform;
		Shared<Core> core;

		State()
		{
			platform = Platform::create();
			core = create_main_core(*platform);
		}

		~State()
		{
			core = nullptr;
		}

	};

	static State* g_state = nullptr;

	void state_frame()
	{
		g_state->platform->poll_events();
		g_state->core->update();

		auto& render = g_state->platform->render;
		if (render.begin_scene())
		{
			g_state->core->draw();
			render.end_scene();
		}
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