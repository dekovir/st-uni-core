#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/renderer/SpriteBatch.hpp"
#include "example.hpp"

namespace unicore
{
	class ReadFileProvider;

	class MyApp : public SDLApplication
	{
	public:
		explicit MyApp(const CoreSettings& settings);

	protected:
		Shared<Example> _example;
		int _example_index = -1;

		Shared<Font> _font;
		Shared<ReadFileProvider> _archive;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;

		uint32_t _draw_calls = 0;

		List<String32> _lines;

		void on_init() override;
		void on_update() override;
		void on_draw() override;

		void set_example(int index);
	};
}