#pragma once
#include "unicore/Core.hpp"

namespace unicore
{
	class MyCore : public Core
	{
	public:
		explicit MyCore(Platform& platform);

	protected:
		Shared<Texture> _tex;
		Radians _angle;

		void on_update() override;
		void on_draw() override;
	};
}
