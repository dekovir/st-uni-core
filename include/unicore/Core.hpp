#pragma once
#include "unicore/Platform.hpp"

namespace unicore
{
	class Core
	{
	public:
		explicit Core(Platform& platform);

		Logger& logger;
		Time& time;
		Input& input;
		Render2D& render;
		FileSystem& file_system;
		Platform& platform;

		virtual ~Core() = default;

		void update();
		void draw();

	protected:
		virtual void on_update() = 0;
		virtual void on_draw() = 0;
	};

	typedef Shared<Core> (*CoreFactory)(Platform& platform);
}