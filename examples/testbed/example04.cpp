#include "example04.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example04, "SpriteBatch");

	Example04::Example04(const ExampleContext& context)
		: Example(context)
	{
	}

	Example04::~Example04()
	{
	}

	bool Example04::load(ResourceCache& resources)
	{
		return true;
	}

	void Example04::update()
	{
		_sprite_batch
			.clear()
			.draw({ 100, 100, 150, 50 }, ColorConst4b::Cyan)
			.flush();
	}

	void Example04::draw() const
	{
		_sprite_batch.render(renderer);
	}
}