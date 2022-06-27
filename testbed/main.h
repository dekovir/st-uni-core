#pragma once
#include "unicore/Core.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class MyCore : public Core
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Shared<Texture> _tex;
		Shared<BitmapFont> _font;
		Radians _angle;

		void on_update() override;
		void on_draw() override;
	};
}
