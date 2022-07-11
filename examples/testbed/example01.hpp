#pragma once
#include "example.hpp"
#include "unicore/Graphics2D.hpp"

namespace unicore
{
	class Example01 : public Example
	{
	public:
		explicit Example01(const ExampleContext& context)
			: Example(context)
		{
		}

		bool load(ResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		Graphics2D _graphics;
	};
}