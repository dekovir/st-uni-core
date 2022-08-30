#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
	class MemoryChunk;

	enum class SeekMethod
	{
		Begin,
		Current,
		End,
	};

	class BasicStream : public Object
	{
		UC_OBJECT(BasicStream, Object)
	public:
		UC_NODISCARD virtual int64_t size() const = 0;
		virtual int64_t seek(int64_t offset, SeekMethod method = SeekMethod::Begin) = 0;
	};

	class ReadStream : public BasicStream
	{
		UC_OBJECT(ReadStream, BasicStream)
	public:
		UC_NODISCARD virtual bool eof() const = 0;
		virtual bool read(void* buffer, size_t size, size_t* bytes_read = nullptr) = 0;

		bool read(MemoryChunk& chunk, size_t* bytes_read = nullptr);
	};

	class WriteStream : public ReadStream
	{
		UC_OBJECT(WriteStream, ReadStream)
	public:
		virtual bool flush() = 0;
		virtual bool write(const void* buffer, size_t size, size_t* bytes_written = nullptr) = 0;

		bool write(const MemoryChunk& chunk, size_t* bytes_written = nullptr);
	};

	// StreamReader ///////////////////////////////////////////////////////////////
	class StreamReader
	{
	public:
		ReadStream& stream;

		explicit StreamReader(ReadStream& stream_)
			: stream(stream_) {}

		UC_NODISCARD bool eof() const { return stream.eof(); }

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		bool read(T& value)
		{
			return stream.read(&value, sizeof(T));
		}

		template<typename Char>
		bool read_string_nt(BasicString<Char>& str)
		{
			if (eof()) return false;

			size_t count = 0;
			Char c;

			while (stream.read(&c, sizeof(Char)) == 1)
			{
				if (c == 0)
					break;

				str.append(c);
				count++;
			}

			return count > 0;
		}
	};

	// StreamWriter ///////////////////////////////////////////////////////////////
	class StreamWriter
	{
	public:
		WriteStream& stream;

		explicit StreamWriter(WriteStream& stream_)
			: stream(stream_)
		{
		}

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		StreamWriter& write(T value)
		{
			stream.write(&value, sizeof(value));
			return *this;
		}

		StreamWriter& write(StringView str)
		{
			stream.write(str.data(), sizeof(char) * str.size());
			return *this;
		}

		StreamWriter& write(WStringView str)
		{
			stream.write(str.data(), sizeof(wchar_t) * str.size());
			return *this;
		}

		template<typename Char>
		StreamWriter& write_string_nt(BasicStringView<Char> str)
		{
			stream.write(str.data(), sizeof(Char) * (str.size() + 1));
			return *this;
		}
	};
}