#pragma once
#include "unicore/app/Core.hpp"
#include "unicore/Console.hpp"
#include "unicore/Random.hpp"
#include "unicore/Font.hpp"
#include "unicore/Sprite.hpp"
#include "unicore/SpriteBatch.hpp"

namespace unicore
{
	class WAEnvironment;
	class WARuntime;
	class WAModule;

	class MyCore : public Core
	{
	public:
		explicit MyCore(const CoreSettings& settings);
		~MyCore() override;

		//protected:
		Shared<Font> _font;
		DefaultConsole _console;
		Shared<Sprite> _spr;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;

		Shared<WAEnvironment> _env;
		Shared<WARuntime> _runtime;
		Shared<WAModule> _module;

		int _sprite_count = 0;
		TimeSpan _update_time, _draw_time;

		void on_update() override;
		void on_draw() override;
	};
}