#include "GenericTime.hpp"

namespace unicore
{
	GenericTime::GenericTime()
	{
		_prev = clock();
	}

	const TimeSpan& GenericTime::elapsed() const
	{
		return _elapsed;
	}

	const TimeSpan& GenericTime::delta() const
	{
		return _delta;
	}

	void GenericTime::update()
	{
		const auto cur = clock();
		const auto delta = static_cast<float>(cur - _prev) / CLOCKS_PER_SEC;

		_delta = TimeSpan::from_seconds(delta);
		_elapsed += _delta;
	}
}