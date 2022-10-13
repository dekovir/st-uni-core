#pragma once
#include "unicore/app/SDLCore.hpp"
#include "unicore/math/Random.hpp"
#include "unicore/system/Console.hpp"
#include "unicore/renderer/Font.hpp"
#include "unicore/renderer/Sprite.hpp"
#include "unicore/renderer/SpriteBatch.hpp"

namespace unicore
{
	class WasmEnvironment;
	class WasmRuntime;
	class WasmModule;

	class MyCore : public SDLCore
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

		Shared<WasmEnvironment> _env;
		Shared<WasmRuntime> _runtime;
		Shared<WasmModule> _module;

		int _sprite_count = 0;
		TimeSpan _update_time, _draw_time;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}