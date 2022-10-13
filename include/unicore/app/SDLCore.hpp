#pragma once
#include "unicore/app/RendererCore.hpp"
#include "unicore/renderer/sdl2/Pipeline.hpp"

namespace unicore
{
	class SDLCore : public RendererCoreT<sdl2::Pipeline>
	{
	public:
		explicit SDLCore(const DisplayCoreSettings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(Platform& platform);
}