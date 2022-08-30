#pragma once
#include "unicore/Stream.hpp"

namespace unicore
{
	class BinaryWriter
	{
	public:
		explicit BinaryWriter(WriteStream& stream)
			: _stream(stream)
		{
		}

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		BinaryWriter& write(T value)
		{
			_stream.write(&value, sizeof(value));
			return *this;
		}

		template<typename Char>
		BinaryWriter& write_string_nt(BasicString<Char>& str)
		{
			_stream.write(str.data(), sizeof(Char) * (str.size() + 1));
			return *this;
		}

	protected:
		WriteStream& _stream;
	};
}