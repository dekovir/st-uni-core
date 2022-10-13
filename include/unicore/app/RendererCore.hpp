#pragma once
#include "unicore/app/DisplayCore.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/renderer/Renderer.hpp"

namespace unicore
{
	class FPSCounter
	{
	public:
		UC_NODISCARD constexpr int current() const { return _current; }

		void update(const TimeSpan& delta)
		{
			_time += delta;
			if (_time >= TimeSpanConst::OneSecond)
			{
				_current = _counter;

				_counter = 0;
				_time -= TimeSpanConst::OneSecond;
			}
		}

		void frame() { _counter++; }

	protected:
		int _current = 0;
		int _counter = 0;
		TimeSpan _time = TimeSpanConst::Zero;
	};

	class RendererCore : public DisplayCore
	{
		ProxyLogger _renderer_logger;
	public:
		using RendererFactory = std::function<Renderer& (Logger& logger, Display& display)>;

		Renderer& renderer;

		explicit RendererCore(const DisplayCoreSettings& settings, const RendererFactory& renderer_factory);

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
	class RendererCoreT : public RendererCore
	{
	public:
		using RendererFactoryT = std::function<RendererType& (Logger& logger, Display& display)>;

		RendererType& renderer;

		explicit RendererCoreT(const DisplayCoreSettings& settings, const RendererFactoryT& renderer_factory)
			: RendererCore(settings, renderer_factory)
			, renderer(static_cast<RendererType&>(RendererCore::renderer))
		{
		}
	};
}