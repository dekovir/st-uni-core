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
		FlushFileBuffers(_handle);
		CloseHandle(_handle);
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

	size_t WinStream::read(void* buffer, size_t size, size_t count)
	{
		size_t cnt = 0;
		const auto ptr = static_cast<char*>(buffer);
		for (size_t i = 0; i < count; i++)
		{
			if (!ReadFile(_handle, &ptr[size * i], size, nullptr, nullptr))
				break;

			cnt++;
		}

		return cnt;
	}

	size_t WinStream::write(const void* buffer, size_t size, size_t count)
	{
		size_t cnt = 0;
		const auto ptr = static_cast<const char*>(buffer);
		for (size_t i = 0; i < count; i++)
		{
			if (WriteFile(_handle, &ptr[size * i], size, nullptr, nullptr))
				break;

			cnt++;
		}

		return cnt;
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