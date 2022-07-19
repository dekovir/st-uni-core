#pragma once
#include "unicore/app/RendererCore.hpp"
#include "unicore/SDLRenderer.hpp"

namespace unicore
{
	class SDLCore : public RendererCoreT<SDLRenderer>
	{
	public:
		explicit SDLCore(const DisplayCoreSettings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(Platform& platform);
}