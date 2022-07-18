#pragma once
#include "unicore/TimeSpan.hpp"
#include "unicore/Platform.hpp"
#include "unicore/Time.hpp"
#include "unicore/Context.hpp"
#include "unicore/SDLRenderer.hpp"

namespace unicore
{
	class Plugin;
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

	template<typename RendererType = Renderer>
	class RendererCore : public Core
	{
	public:
		RendererType& renderer;

		explicit RendererCore(const Settings& settings, RendererType& renderer_)
			: Core(settings)
			, renderer(renderer_)
		{
			renderer.register_module(context);
		}

		~RendererCore() override
		{
			renderer.unregister_module(context);
		}

		UC_NODISCARD constexpr int fps() const { return _fps_current; }

		void update() override
		{
			Core::update();

			_fps_time += time.delta();
			if (_fps_time >= TimeSpanConst::OneSecond)
			{
				_fps_current = _fps_counter;

				_fps_counter = 0;
				_fps_time -= TimeSpanConst::OneSecond;
			}
		}

		virtual void draw()
		{
			_fps_counter++;
			if (renderer.begin_scene())
			{
				on_draw();
				renderer.end_scene();
			}
		}

		virtual void frame()
		{
			update();
			draw();
		}

	protected:
		virtual void on_draw() = 0;

	private:
		int _fps_current = 0;
		int _fps_counter = 0;
		TimeSpan _fps_time = TimeSpanConst::Zero;
	};

	class SDLCore : public RendererCore<SDLRenderer>
	{
	public:
		explicit SDLCore(const Settings& settings);
		~SDLCore() override;
	};

	typedef Shared<Core>(*CoreFactory)(const Core::Settings& settings);
}