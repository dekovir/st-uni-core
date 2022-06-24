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
	public:
		UC_NODISCARD virtual int64_t size() const = 0;
		virtual int64_t seek(int64_t offset, SeekMethod method = SeekMethod::Begin) = 0;
	};

	class ReadStream : virtual public BasicStream
	{
	public:
		UC_NODISCARD virtual bool eof() const = 0;
		virtual size_t read(void* buffer, size_t size, size_t count = 1) = 0;
	};

	class WriteStream : virtual public BasicStream
	{
	public:
		virtual size_t write(const void* buffer, size_t size, size_t count = 1) = 0;
		virtual bool flush() = 0;
	};
}