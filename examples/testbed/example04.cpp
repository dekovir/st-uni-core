#include "example04.hpp"

namespace unicore
{
	UC_EXAMPLE_REGISTER(Example04, "Empty");

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
	}

	void Example04::draw() const
	{
	}
}