#pragma once
#include "unicore/app/DisplayApplication.hpp"
#include "unicore/system/TimeSpan.hpp"
#include "unicore/renderer/FPSCounter.hpp"
#include "unicore/renderer/Renderer.hpp"

namespace unicore
{
	class RendererApplication : public DisplayApplication
	{
		ProxyLogger _renderer_logger;
	public:
		using RendererFactory = std::function<Renderer& (Logger& logger, Display& display)>;

		Renderer& renderer;

		explicit RendererApplication(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory);

		UC_NODISCARD constexpr int fps() const { return _fps_counter.current(); }

		void update() override;

		virtual void draw();
		virtual void frame();

	protected:
		FPSCounter _fps_counter;

		virtual void on_draw() = 0;
	};

	template<typename RendererType,
		std::enable_if_t<std::is_base_of_v<Renderer, RendererType>>* = nullptr>
	class RendererCoreT : public RendererApplication
	{
	public:
		using RendererFactoryT = std::function<RendererType& (Logger& logger, Display& display)>;

		RendererType& renderer;

		explicit RendererCoreT(const DisplayCoreSettings& settings, const RendererFactoryT& renderer_factory)
			: RendererApplication(settings, renderer_factory)
			, renderer(static_cast<RendererType&>(RendererApplication::renderer))
		{
		}
	};
}