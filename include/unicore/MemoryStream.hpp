#pragma once
#include "unicore/Stream.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class MemoryStream : public ReadStream
	{
		UC_OBJECT(MemoryStream, ReadStream)
	public:
		explicit MemoryStream(const Shared<MemoryChunk>& chunk);

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

	protected:
		Shared<MemoryChunk> _chunk;
		int64_t _position = 0;
	};
}