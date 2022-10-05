#pragma once
#include "unicore/app/SDLCore.hpp"
#include "unicore/Graphics2D.hpp"
#include "unicore/SpriteBatch.hpp"

namespace unicore
{
	class MyCore : public SDLCore
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Graphics2D _graphics;
		SpriteBatch _sprite_batch;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}