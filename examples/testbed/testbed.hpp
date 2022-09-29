#pragma once
#include "unicore/app/SDLCore.hpp"
#include "unicore/SpriteBatch.hpp"
#include "example.hpp"

namespace unicore
{
	class MyCore : public SDLCore
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Shared<Example> _example;
		int _example_index = -1;

		Shared<Font> _font;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;

		uint32_t _draw_calls = 0;

		List<WString> _lines;

		void on_init() override;
		void on_update() override;
		void on_draw() override;

		void set_example(int index);
	};
}