#pragma once
#include "unicore/Stream.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class ReadMemoryStream : public ReadStream
	{
		UC_OBJECT(ReadMemoryStream, ReadStream)
	public:
		explicit ReadMemoryStream(const Shared<MemoryChunk>& chunk);

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

	protected:
		Shared<MemoryChunk> _chunk;
		int64_t _position = 0;
	};

	class WriteMemoryStream : public WriteStream
	{
		UC_OBJECT(WriteMemoryStream, WriteStream)
	public:
		WriteMemoryStream() = default;

		UC_NODISCARD int64_t size() const override;

		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

		bool flush() override;
		bool write(const void* buffer, size_t size, size_t* bytes_written) override;

	protected:
		List<uint8_t> _bytes;
		int64_t _position = 0;
	};
}