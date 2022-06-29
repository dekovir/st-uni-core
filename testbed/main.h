#pragma once
#include "unicore/Core.hpp"
#include "unicore/Font.hpp"

namespace unicore
{
	struct Entity
	{
		Vector2f position;
		Vector2f velocity = Vector2f(300, 300);

		float radius;
		Vector2f scale;

		Radians angle;
		Radians aspeed;

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

		void on_update() override;
		void on_draw() override;

		void spawn_entity(const Vector2f& position, const Vector2i& size);
	};
}