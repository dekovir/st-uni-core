#pragma once
#include "example.hpp"

namespace unicore
{
	class Example04 : public Example
	{
	public:
		explicit Example04(const ExampleContext& context);
		~Example04() override;

		bool load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;
	};
}