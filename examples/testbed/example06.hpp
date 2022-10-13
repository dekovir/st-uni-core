#pragma once
#include "example.hpp"
#include "unicore/renderer/SpriteBatch.hpp"
#include "unicore/renderer/Graphics2D.hpp"

namespace unicore
{
	enum class CellType : uint16_t
	{
		Empty = 0,
		Grass = 1,
		Solid = 7,
	};

	class Example06 : public Example
	{
	public:
		explicit Example06(const ExampleContext& context);

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		Shared<Font> _font;
		Graphics2D _graphics;
		SpriteBatch _sprite_batch;
	};
}