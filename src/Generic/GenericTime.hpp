#pragma once
#include "unicore/Time.hpp"
#include <time.h>

namespace unicore
{
	class GenericTime : public Time
	{
		UC_OBJECT(GenericTime, Time)
	public:
		GenericTime();

		UC_NODISCARD const TimeSpan& elapsed() const override;
		UC_NODISCARD const TimeSpan& delta() const override;

		void update();

	protected:
		clock_t _prev;
		TimeSpan _elapsed, _delta;
	};
}