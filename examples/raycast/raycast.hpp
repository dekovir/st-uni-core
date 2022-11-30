#pragma once
#include "unicore/app/SDLApplication.hpp"
#include "unicore/system/Buffer2.hpp"
#include "unicore/renderer/PrimitiveBatch.hpp"
#include "unicore/renderer/SpriteBatch.hpp"

namespace unicore
{
	enum class CellType
	{
		Floor,
		Wall,
	};

	struct Cell
	{
		static constexpr float WorldSide = 10;

		CellType type;
	};

	using Map = Buffer2<Cell>;

	class Entity
	{
	public:
		Vector2f position;
		Radians angle;

		UC_NODISCARD Vector2f forward() const
		{
			return VectorConst2f::AxisX.rotate(angle);
		}

		void move_forward(float amount)
		{
			position += forward() * amount;
		}
	};

	class Player : public Entity
	{
	public:
	};

	class MyCore : public SDLApplication
	{
	public:
		explicit MyCore(const CoreSettings& settings);

	protected:
		PrimitiveBatch _graphics;
		SpriteBatch _sprite_batch;

		Shared<Font> _font;

		Shared<Map> _map;
		Shared<Player> _player;

		void on_init() override;
		void on_update() override;
		void on_draw() override;
	};
}