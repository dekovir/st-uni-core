#pragma once
#include "unicore/Core.hpp"
#include "unicore/Font.hpp"
#include "unicore/Graphics2D.hpp"

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
		Shared<Texture> _tex;
		Shared<BitmapFont> _font;
		DefaultRandom _random;
		List<Entity> _entites;
		Graphics2D _graphics;

		void on_update() override;
		void on_draw() override;

		void spawn_entity(const Vector2f& position, const Vector2i& size);
		void spawn_entities(unsigned count);
	};
}