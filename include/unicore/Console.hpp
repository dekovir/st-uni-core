#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Buffer2.hpp"

namespace unicore
{
	enum class ConsoleColor8 : uint8_t
	{
		Black = 0,

		ForegroundRed = 1 << 0,
		ForegroundGreen = 1 << 1,
		ForegroundBlue = 1 << 2,
		ForegroundBright = 1 << 3,

		BackgroundRed = 1 << 4,
		BackgroundGreen = 1 << 5,
		BackgroundBlue = 1 << 6,
		BackgroundBright = 1 << 7,

		ForegroundYellow = ForegroundRed | ForegroundGreen,
		ForegroundMagenta = ForegroundRed | ForegroundBlue,
		ForegroundCyan = ForegroundGreen | ForegroundBlue,
		ForegroundWhite = ForegroundRed | ForegroundGreen | ForegroundBlue,

		BackgroundYellow = BackgroundRed | BackgroundGreen,
		BackgroundMagenta = BackgroundRed | BackgroundBlue,
		BackgroundCyan = BackgroundGreen | BackgroundBlue,
		BackgroundWhite = BackgroundRed | BackgroundGreen | BackgroundBlue,
	};
	UNICORE_ENUM_FLAGS(ConsoleColor8, ConsoleColor8Value);

	static_assert(sizeof(ConsoleColor8) == 1);
	static_assert(sizeof(ConsoleColor8Value) == 1);

	template<typename TChar, typename TColor>
	struct ConsoleChar
	{
		TChar c;
		TColor color;
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

		Console& new_line()
		{
			_position.x = 0;
			_position.y++;
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

	using DefaultConsoleChar = ConsoleChar<char, ConsoleColor8Value>;
	using DefaultConsole = Console<DefaultConsoleChar>;

	using DefaultConsoleCharW = ConsoleChar<wchar_t, ConsoleColor8Value>;
	using DefaultConsoleW = Console<DefaultConsoleCharW>;

	class ConsoleLogger : public Logger
	{
	public:
		using ConsoleType = Console<ConsoleChar<char, ConsoleColor8Value>>;

		explicit ConsoleLogger(ConsoleType& console)
			: _console(console)
		{}

		void write(LogType type, const StringView text) override
		{
			const auto color = get_color(type);

			for (const auto c : text)
			{
				const auto pos = _console.get_position();
				_console.set(pos.x, pos.y, { c, color });
				_console.set_position(pos.x + 1, pos.y);
			}

			_console.new_line();
		}

	protected:
		ConsoleType& _console;

		UC_NODISCARD static constexpr ConsoleColor8 get_color(LogType type)
		{
			switch (type)
			{
			case LogType::Info: return ConsoleColor8::ForegroundWhite;
			case LogType::Debug: return ConsoleColor8::ForegroundBlue;
			case LogType::Warning: return ConsoleColor8::ForegroundYellow;
			case LogType::Error: return ConsoleColor8::ForegroundRed;
			}

			return ConsoleColor8::ForegroundMagenta;
		}
	};
}