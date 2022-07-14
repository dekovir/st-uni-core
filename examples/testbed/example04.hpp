#pragma once
#include "example.hpp"
#if defined(UNICORE_USE_WASM)
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

	class Example04 : public Example
	{
	public:
		explicit Example04(const ExampleContext& context);
		~Example04() override;

		bool load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;

	//protected:
		Shared<WAEnvironment> _env;
		Shared<WARuntime> _runtime;
		Shared<WAModule> _module;

		DefaultConsole _console;
		Shared<Font> _font;
		Shared<Sprite> _spr;
		SpriteBatch _sprite_batch;
		DefaultRandom _random;
	};
}
#endif