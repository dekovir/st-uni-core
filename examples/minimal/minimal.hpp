#pragma once
#include "unicore/app/SDLCore.hpp"
#include "unicore/Graphics2D.hpp"
#include "unicore/SpriteBatch.hpp"
#include "unicore/RendererSDL.hpp"

namespace unicore
{
	struct Entity
	{
		Vector2f center;
		Vector2f velocity = Vector2f(300, 300);

		float radius;
		Vector2f scale;

		Radians angle;
		Radians angle_speed;

		Color4b color;

		void update(const Vector2i& size, float delta);
	};

	class MyCore : public SDLCore
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Shared<Texture> _tex;
		DefaultRandom _random;
		List<Entity> _entites;

		Graphics2D _graphics;
		SpriteBatch _sprite_batch;

		TimeSpan _add_time = TimeSpanConst::Zero;
		bool _add_active = false;

		void on_update() override;
		void on_draw() override;

		void spawn_entity(const Vector2f& position, const Vector2i& size);
		void spawn_entities(unsigned count);
	};
}