#pragma once
#include "unicore/Platform.hpp"
#if defined(UNICORE_USE_SDL2)
#include "unicore/Time.hpp"
#include <SDL.h>

namespace unicore
{
	class SDL2Time : public Time
	{
	public:
		SDL2Time();

		UC_NODISCARD const TimeSpan& delta() const override { return _delta; }
		UC_NODISCARD const TimeSpan& elapsed() const override { return  _elapsed; }

		void update();

	protected:
		Uint32 _prevTicks = 0;
		TimeSpan _elapsed = TimeSpanConst::Zero;
		TimeSpan _delta = TimeSpanConst::Zero;
	};
}
#endif