#include "unicore/MemoryFile.hpp"
#include "unicore/Math.hpp"

namespace unicore
{
	// ReadMemoryFile /////////////////////////////////////////////////////////////
	ReadMemoryFile::ReadMemoryFile(const Shared<MemoryChunk>& chunk)
		: _chunk(chunk)
	{
	}

	int64_t ReadMemoryFile::size() const
	{
		return static_cast<int64_t>(_chunk->size());
	}

	int64_t ReadMemoryFile::seek(int64_t offset, SeekMethod method)
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
			_position = size() - offset;
			break;
		}

		_position = Math::clamp<int64_t>(_position, 0, size());
		return _position;
	}

	bool ReadMemoryFile::eof() const
	{
		return _position >= static_cast<int64_t>(_chunk->size());
	}

	bool ReadMemoryFile::read(void* buffer, size_t size, size_t* bytes_read)
	{
		const auto count = Math::min<size_t>(size, _chunk->size() - _position);
		if (bytes_read) *bytes_read = count;
		if (count > 0)
		{
			Memory::copy(buffer, _chunk->data(), count);
			return true;
		}

		return false;
	}

	// WriteMemoryFile ////////////////////////////////////////////////////////////
	int64_t WriteMemoryFile::size() const
	{
		return static_cast<int64_t>(_bytes.size());
	}

	int64_t WriteMemoryFile::seek(int64_t offset, SeekMethod method)
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
			_position = size() - offset;
			break;
		}

		_position = Math::clamp<int64_t>(_position, 0, size());
		return _position;
	}

	bool WriteMemoryFile::eof() const
	{
		return _position >= size();
	}

	bool WriteMemoryFile::read(void* buffer, size_t size, size_t* bytes_read)
	{
		const auto count = Math::min(static_cast<int64_t>(size), this->size() - _position);
		Memory::copy(buffer, _bytes.data() + _position, count);
		if (bytes_read) *bytes_read = count;
		return true;
	}

	bool WriteMemoryFile::flush()
	{
		return true;
	}

	bool WriteMemoryFile::write(const void* buffer, size_t size, size_t* bytes_written)
	{
		_bytes.resize(_position + size);
		Memory::copy(_bytes.data() + _position, buffer, size);
		if (bytes_written) *bytes_written = size;
		return true;
	}
}