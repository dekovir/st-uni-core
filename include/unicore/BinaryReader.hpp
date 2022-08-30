#pragma once
#include "unicore/Stream.hpp"

namespace unicore
{
	class BinaryReader
	{
	public:
		explicit BinaryReader(ReadStream& stream)
			: _stream(stream) {}

		UC_NODISCARD bool eof() const { return _stream.eof(); }

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		bool read(T& value)
		{
			return _stream.read(&value, sizeof(T));
		}

		template<typename Char>
		bool read_string_nt(BasicString<Char>& str)
		{
			if (eof()) return false;

			size_t count = 0;
			Char c;

			while (_stream.read(&c, sizeof(Char)) == 1)
			{
				if (c == 0)
					break;

				str.append(c);
				count++;
			}

			return count > 0;
		}

	protected:
		ReadStream& _stream;
	};
}