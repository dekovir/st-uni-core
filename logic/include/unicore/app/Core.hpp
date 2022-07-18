#pragma once
#include "unicore/Platform.hpp"
#include "unicore/TimeSpan.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	class Plugin;
	class Renderer;
	class SDLRenderer;

	class Core
	{
	public:
		struct Settings
		{
			Platform& platform;
		};

		explicit Core(const Settings& settings);
		virtual ~Core();

		Platform& platform;
		Logger& logger;
		Time& time;
		Input& input;
		ResourceCache& resources;

		Context context;

		virtual void update();

	protected:
		virtual void on_update() = 0;

	private:
		List<Unique<Plugin>> _plugins;
	};

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

	template<typename RendererType>
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

	class SDLCore : public RendererCoreT<SDLRenderer>
	{
	public:
		explicit SDLCore(const Settings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(const Core::Settings& settings);
}