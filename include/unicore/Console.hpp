#pragma once
#include "unicore/Buffer2.hpp"

namespace unicore
{
	enum class ColorEnum8 : uint8_t
	{
		Black,
		Red,
		Green,
		Blue,
		Yellow,
		Magenta,
		Cyan,
		White
	};

	struct ConsoleColor8
	{
		ColorEnum8 fg : 3;
		uint8_t fg_bright : 1;

		ColorEnum8 bg : 3;
		uint8_t bg_bright : 1;

		constexpr ConsoleColor8()
			: fg(ColorEnum8::White), fg_bright(0)
			, bg(ColorEnum8::Black), bg_bright(0)
		{
		}
	};

	static_assert(sizeof(ConsoleColor8) == 1);

	template<typename TChar, typename TColor>
	struct ConsoleChar
	{
		TChar c;
		TColor bc, fc;
	};

	template<typename TChar>
	class Console : public Buffer2<TChar>
	{
	public:
		explicit Console(const Vector2i& size)
			: Buffer2<TChar>(size)
		{
		}

		Console(int x, int y)
			: Buffer2<TChar>(x, y)
		{
		}

		UC_NODISCARD constexpr const Vector2i& get_position() const { return _position; }

		Console& print(BasicStringView<TChar> text)
		{
			for (const auto& c : text)
			{
				set(_position.x, _position.y, c);
				_position.x++;
			}

			return *this;
		}

		Console& set_position(int x, int y)
		{
			_position.set(x, y);
			return *this;
		}

		Console& set_position(const Vector2i& position)
		{
			_position = position;
			return *this;
		}

		Console& reset_position()
		{
			_position.set(0, 0);
			return *this;
		}

	protected:
		Vector2i _position = VectorConst2i::Zero;
	};

	using DefaultConsole = Console<ConsoleChar<wchar_t, ConsoleColor8>>;
}