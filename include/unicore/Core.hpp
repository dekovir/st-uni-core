#pragma once
#include "unicore/Platform.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	struct CoreSettings
	{
		Platform& platform;
		Render2D& render;
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
		Render2D& render;

		Context context;

		void update();
		void draw();

		void frame();

	protected:
		virtual void on_update() = 0;
		virtual void on_draw() = 0;
	};

	typedef Shared<Core>(*CoreFactory)(const CoreSettings& settings);
}