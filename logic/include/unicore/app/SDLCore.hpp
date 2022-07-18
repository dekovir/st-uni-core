#pragma once
#include "unicore/app/RendererCore.hpp"
#include "unicore/SDLRenderer.hpp"

namespace unicore
{
	class SDLCore : public RendererCoreT<SDLRenderer>
	{
	public:
		explicit SDLCore(const Settings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(const Core::Settings& settings);
}