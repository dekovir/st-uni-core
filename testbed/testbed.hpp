#pragma once
#include "unicore/app/Core.hpp"
#include "unicore/SpriteBatch.hpp"
#include "unicore/fnt/BitmapFont.hpp"
#include "example.hpp"

namespace unicore
{
	class MyCore : public Core
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Shared<Example> _example;
		int _example_index = -1;

		Shared<BitmapFont> _font;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;

		uint32_t _draw_calls = 0;

		void on_update() override;
		void on_draw() override;

		void set_example(int index);
	};
}