#include "unicore/MemoryStream.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	MemoryStream::MemoryStream(const Shared<MemoryChunk>& chunk)
		: _chunk(chunk)
	{
	}

	int64_t MemoryStream::size() const
	{
		return _chunk->size();
	}

	int64_t MemoryStream::seek(int64_t offset, SeekMethod method)
	{
		switch (method)
		{
		case SeekMethod::Begin:
			_position = offset;
			break;

		case SeekMethod::Current:
			_position += offset;
			break;

		case SeekMethod::End:
			_position = _chunk->size() - offset;
			break;
		}

		_position = Math::clamp<int64_t>(_position, 0, _chunk->size());
		return _position;
	}

	bool MemoryStream::eof() const
	{
		return _position >= _chunk->size();
	}

	bool MemoryStream::read(void* buffer, size_t size, size_t* bytes_read)
	{
		const auto count = Math::min(size, _chunk->size() - _position);
		if (bytes_read) *bytes_read = count;
		if (count > 0)
		{
			Memory::copy(buffer, _chunk->data(), count);
			return true;
		}

		return false;
	}
}