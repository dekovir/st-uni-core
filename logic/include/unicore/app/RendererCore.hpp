#pragma once
#include "unicore/app/Core.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/Renderer.hpp"

namespace unicore
{
	class RendererCore : public Core
	{
	public:
		Renderer& renderer;

		explicit RendererCore(const Settings& settings, Renderer& renderer_);

		~RendererCore() override;

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
		RendererType& renderer;

		explicit RendererCoreT(const Settings& settings, RendererType& renderer_)
			: RendererCore(settings, renderer_)
			, renderer(renderer_)
		{
			renderer.register_module(context);
		}
	};
}