#pragma once
#include "unicore/app/DisplayCore.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	class RendererCore : public DisplayCore
	{
		ProxyLogger _renderer_logger;
	public:
		using RendererFactory = std::function<Renderer&(Logger& logger, Display& display)>;

		Renderer& renderer;

		explicit RendererCore(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory);

		UC_NODISCARD constexpr int fps() const { return _fps_current; }

		void update() override;

		virtual void draw();
		virtual void frame();

	protected:
		virtual void on_draw() = 0;

	private:
		int _fps_current = 0;
		int _fps_counter = 0;
		TimeSpan _fps_time = TimeSpanConst::Zero;
	};

	template<typename RendererType,
		std::enable_if_t<std::is_base_of_v<Renderer, RendererType>>* = nullptr>
	class RendererCoreT : public RendererCore
	{
	public:
		using RendererFactoryT = std::function<RendererType&(Logger& logger, Display& display)>;

		RendererType& renderer;

		explicit RendererCoreT(const DisplayCoreSettings& settings, const RendererFactoryT& renderer_factory)
			: RendererCore(settings, renderer_factory)
			, renderer(static_cast<RendererType&>(RendererCore::renderer))
		{
		}
	};
}