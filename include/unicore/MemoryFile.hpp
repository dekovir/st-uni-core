#pragma once
#include "unicore/File.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class ReadMemoryFile : public ReadFile
	{
		UC_OBJECT(ReadMemoryFile, ReadFile)
	public:
		explicit ReadMemoryFile(const Shared<MemoryChunk>& chunk);

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

	protected:
		Shared<MemoryChunk> _chunk;
		int64_t _position = 0;
	};

	class WriteMemoryFile : public WriteFile
	{
		UC_OBJECT(WriteMemoryFile, WriteFile)
	public:
		explicit WriteMemoryFile(size_t size = 0);

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