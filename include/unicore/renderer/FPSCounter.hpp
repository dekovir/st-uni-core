#pragma once
#include "unicore/system/TimeSpan.hpp"

namespace unicore
{
	class FPSCounter
	{
	public:
		UC_NODISCARD constexpr int current() const { return _current; }

		void update(const TimeSpan& delta)
		{
			_time += delta;
			if (_time >= TimeSpanConst::OneSecond)
			{
				_current = _counter;

				_counter = 0;
				_time -= TimeSpanConst::OneSecond;
			}
		}

		void frame() { _counter++; }

	protected:
		int _current = 0;
		int _counter = 0;
		TimeSpan _time = TimeSpanConst::Zero;
	};
}