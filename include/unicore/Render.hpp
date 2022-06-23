#pragma once
#include "unicore/Module.hpp"
#include "unicore/Vector2.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class Render : public Module
	{
	public:
		explicit Render(Logger& logger) : _logger(logger) {}

		UC_NODISCARD virtual const Vector2i& screen_size() const = 0;

		virtual bool begin_scene() = 0;
		virtual void end_scene() = 0;

	protected:
		Logger& _logger;
	};
}