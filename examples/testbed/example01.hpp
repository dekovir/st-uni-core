#pragma once
#include "example.hpp"
#include "unicore/renderer/Graphics2D.hpp"

namespace unicore
{
	class Example01 : public Example
	{
	public:
		explicit Example01(const ExampleContext& context)
			: Example(context)
		{
		}

		void load(IResourceCache& resources) override;
		void update() override;
		void draw() const override;

	protected:
		Shared<GeometryFont> _font;
		Graphics2D _graphics;
		Radians _angle;
	};
}