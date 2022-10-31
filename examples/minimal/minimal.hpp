#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/math/Random.hpp"
#include "unicore/renderer/PrimitiveBatch.hpp"
#include "unicore/renderer/SpriteBatch.hpp"

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

	class MyApp : public SDLApplication
	{
	public:
		explicit MyApp(const CoreSettings& settings);

	protected:
		Shared<Texture> _tex;
		DefaultRandom _random;
		List<Entity> _entites;

		PrimitiveBatch _graphics;
		SpriteBatch _sprite_batch;

		TimeSpan _add_time = TimeSpanConst::Zero;
		bool _add_active = false;

		void on_init() override;
		void on_update() override;
		void on_draw() override;

		void spawn_entity(const Vector2f& position, const Vector2i& size);
		void spawn_entities(unsigned count);
	};
}