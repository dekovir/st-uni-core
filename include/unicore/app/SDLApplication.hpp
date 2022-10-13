#pragma once
#include "unicore/app/RendererApplication.hpp"
#include "unicore/renderer/sdl2/Pipeline.hpp"

namespace unicore
{
	class SDLApplication : public RendererCoreT<sdl2::Pipeline>
	{
	public:
		explicit SDLApplication(const DisplayCoreSettings& settings);
		~SDLApplication() override;
	};

	typedef Shared<Application>(*CoreFactory)(Platform& platform);
}