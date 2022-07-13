#pragma once
#include "unicore/TimeSpan.hpp"
#include "unicore/Platform.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	class Plugin;

	struct CoreSettings
	{
		Platform& platform;
		Renderer2D& render;
	};

	class Core
	{
	public:
		explicit Core(const CoreSettings& settings);
		virtual ~Core();

		Platform& platform;
		Logger& logger;
		Time& time;
		Input& input;
		ResourceCache& resources;
		Renderer2D& renderer;

		Context context;

		void update();
		void draw();

		void frame();

		UC_NODISCARD constexpr int fps() const { return _fps_current; }

	protected:
		virtual void on_update() = 0;
		virtual void on_draw() = 0;

	private:
		int _fps_current = 0;
		int _fps_counter = 0;
		TimeSpan _fps_time = TimeSpanConst::Zero;

		List<Unique<Plugin>> _plugins;
	};

	typedef Shared<Core>(*CoreFactory)(const CoreSettings& settings);
}