#include "WinStream.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)

namespace unicore
{
	WinStream::WinStream(HANDLE handle)
		: _handle(handle)
	{
	}

	WinStream::~WinStream()
	{
		if (_handle != nullptr)
		{
			FlushFileBuffers(_handle);
			CloseHandle(_handle);
		}
	}

	WinStream::WinStream(WinStream&& other) noexcept
		: _handle(std::exchange(other._handle, nullptr))
	{
	}

	WinStream& WinStream::operator=(WinStream&& other) noexcept
	{
		_handle = std::exchange(other._handle, nullptr);
		return *this;
	}

	int64_t WinStream::size() const
	{
		LARGE_INTEGER size;
		if (GetFileSizeEx(_handle, &size) != 0)
			return size.QuadPart;

		return 0;
	}

	int64_t WinStream::seek(int64_t offset, SeekMethod method)
	{
		LARGE_INTEGER li_offset, li_position;
		li_offset.QuadPart = offset;
		if (SetFilePointerEx(_handle, li_offset, &li_position, convert_method(method)) != 0)
			return li_position.QuadPart;

		return 0;
	}

	bool WinStream::eof() const
	{
		LARGE_INTEGER pos, size;
		if (SetFilePointerEx(_handle, {}, &pos, FILE_CURRENT) && GetFileSizeEx(_handle, &size))
			return pos.QuadPart == size.QuadPart;

		return false;
	}

	bool WinStream::read(void* buffer, size_t size, size_t* bytes_read)
	{
		DWORD count;
		const auto result = ReadFile(_handle, buffer, size, &count, nullptr);
		if (bytes_read)
			*bytes_read = count;
		return result;
	}

	bool WinStream::write(const void* buffer, size_t size, size_t* bytes_written)
	{
		DWORD count;
		const auto result = WriteFile(_handle, buffer, size, &count, nullptr);
		if (bytes_written)
			*bytes_written = count;
		return result;
	}

	bool WinStream::flush()
	{
		return FlushFileBuffers(_handle);
	}

	DWORD WinStream::convert_method(SeekMethod method)
	{
		switch (method)
		{
		case SeekMethod::Begin: return FILE_BEGIN;
		case SeekMethod::Current: return FILE_CURRENT;
		case SeekMethod::End: return FILE_END;
		}

		UC_ASSERT_ALWAYS_MSG("Invalid SeekMethod");
		return FILE_BEGIN;
	}
}
#endif