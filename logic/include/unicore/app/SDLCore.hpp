#pragma once
#include "unicore/app/RendererCore.hpp"
#include "unicore/RendererSDL.hpp"

namespace unicore
{
	class SDLCore : public RendererCoreT<RendererSDL>
	{
	public:
		explicit SDLCore(const DisplayCoreSettings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(Platform& platform);
}