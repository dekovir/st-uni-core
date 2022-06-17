#pragma once
#include "unicore/Core.hpp"
#include "unicore/RenderQueue.hpp"

namespace unicore
{
	class MyCore : public Core
	{
	public:
		explicit MyCore(Platform& platform);

	protected:
		RenderQueue _queue;
		Shared<Texture> _tex;
		Radians _angle;

		void on_update() override;
		void on_draw() override;
	};
}
