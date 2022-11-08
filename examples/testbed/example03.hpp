#pragma once
#include "example.hpp"

namespace unicore
{
	class Example03 : public Example
	{
	public:
		explicit Example03(const ExampleContext& context);

		void update() override;
		void draw() const override;
	};
}