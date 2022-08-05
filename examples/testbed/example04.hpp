#pragma once
#include "example.hpp"
#include "unicore/SpriteBatch.hpp"

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

	protected:
		SpriteBatch _sprite_batch;
	};
}