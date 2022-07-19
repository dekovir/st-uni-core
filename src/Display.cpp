#include "unicore/Display.hpp"

namespace unicore
{
	Display::Display(const DisplaySettings& settings)
		: _logger(settings.logger)
	{}
}