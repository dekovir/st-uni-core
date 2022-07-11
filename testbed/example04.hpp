#pragma once
#include "example.hpp"
#if defined(UNICORE_USE_WASM)
#include "unicore/Console.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	class Example04 : public Example
	{
	public:
		explicit Example04(const ExampleContext& context);

		bool load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		DefaultConsole _console;
		Shared<Font> _font;
	};
}
#endif