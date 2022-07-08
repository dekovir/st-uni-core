#include "example.hpp"

namespace unicore
{
	List<ExampleInfo> ExampleCatalog::_examples;

	Example::Example(const ExampleContext& context)
		: logger(context.logger)
		, random(context.random)
		, time(context.time)
		, input(context.input)
		, render(context.render)
	{}
}