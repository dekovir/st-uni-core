#pragma once
#include "unicore/Object.hpp"

namespace unicore
{
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

	class ReadStream : virtual public BasicStream
	{
		UC_OBJECT(ReadStream, Object)
	public:
		UC_NODISCARD virtual bool eof() const = 0;
		virtual bool read(void* buffer, size_t size, size_t* bytes_read = nullptr) = 0;
	};

	class WriteStream : virtual public BasicStream
	{
		UC_OBJECT(WriteStream, Object)
	public:
		virtual bool flush() = 0;
		virtual bool write(const void* buffer, size_t size, size_t* bytes_written = nullptr) = 0;
	};
}