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
	class Console : public IBuffer2<TChar>
	{
	public:
	};

	using DefaultConsole = Console<ConsoleChar<char, ConsoleColor8>>;
	using DefaultConsoleW = Console<ConsoleChar<wchar_t, ConsoleColor8>>;
}