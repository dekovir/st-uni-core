#pragma once
#include "unicore/Object.hpp"
#include "unicore/Stream.hpp"

namespace unicore
{
	class StreamWriter : public Object
	{
	public:
		explicit StreamWriter(WriteStream& stream);

		StreamWriter& write(const StringView text);

		StreamWriter& write(const WStringView text);

		template<typename T, std::enable_if_t<std::is_integral<T>::type>>
		inline StreamWriter& write(const T value)
		{
			_stream.write(&value, sizeof(T), 1);
			return *this;
		}

	protected:
		WriteStream& _stream;
	};
}