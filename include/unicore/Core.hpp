#pragma once
#include "unicore/Platform.hpp"
#include "unicore/Context.hpp"

namespace unicore
{
	class Core
	{
	public:
		explicit Core(Platform& platform);
		virtual ~Core();

		Logger& logger;
		Time& time;
		Input& input;
		Render2D& render;
		Platform& platform;
		ResourceCache& resources;

		Context context;

		void update();
		void draw();

	protected:
		virtual void on_update() = 0;
		virtual void on_draw() = 0;
	};

	typedef Shared<Core>(*CoreFactory)(Platform& platform);
}