#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/renderer/PrimitiveBatch.hpp"
#include "unicore/renderer/SpriteBatch.hpp"

namespace unicore
{
	class MyCore : public SDLApplication
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		PrimitiveBatch _graphics;
		SpriteBatch _sprite_batch;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}