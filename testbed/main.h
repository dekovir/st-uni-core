#pragma once
#include "unicore/Font.hpp"
#include "unicore/Graphics2D.hpp"
#include "unicore/SpriteBatch.hpp"
#include "unicore/app/Core.hpp"
#include "unicore/fnt/BitmapFont.hpp"

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

	class MyCore : public Core
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		Shared<Texture> _tex1, _tex2;
		Shared<BitmapFont> _font;
		DefaultRandom _random;
		List<Entity> _entites;

		Graphics2D _graphics;
		SpriteBatch _sprite_batch;

		uint32_t _draw_calls = 0;

		TimeSpan _add_time = TimeSpanConst::Zero;
		bool _add_active = false;

		void on_update() override;
		void on_draw() override;

		void spawn_entity(const Vector2f& position, const Vector2i& size);
		void spawn_entities(unsigned count);
	};
}