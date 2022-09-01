#include "LinuxFile.hpp"
#if defined(UNICORE_PLATFORM_LINUX)

namespace unicore
{
	LinuxFile::LinuxFile(FILE* handle)
		: _handle(handle)
	{
	}

	LinuxFile::~LinuxFile()
	{
		fflush(_handle);
		fclose(_handle);
	}

	int64_t LinuxFile::size() const
	{
		const auto current_pos = ftell(_handle);
		fseek(_handle, 0, SEEK_END);
		const auto size = ftell(_handle);
		fseek(_handle, current_pos, SEEK_SET);
		return size;
	}

	int64_t LinuxFile::seek(int64_t offset, SeekMethod method)
	{
		return fseek(_handle, offset, convert_method(method));
	}

	bool LinuxFile::eof() const
	{
		return feof(_handle) != 0;
	}

	bool LinuxFile::read(void* buffer, size_t size, size_t* bytes_read)
	{
		const auto result = fread(buffer, 1, size, _handle);
		if (bytes_read)
			*bytes_read = result;
		return result == size;
	}

	bool LinuxFile::flush()
	{
		return fflush(_handle) == 0;
	}

	bool LinuxFile::write(const void* buffer, size_t size, size_t* bytes_written)
	{
		const auto result = fwrite(buffer, 1, size, _handle);
		if (bytes_written)
			*bytes_written = result;
		return result == size;
	}

	int LinuxFile::convert_method(SeekMethod method)
	{
		switch (method)
		{
		case SeekMethod::Begin: return SEEK_SET;
		case SeekMethod::Current: return SEEK_CUR;
		case SeekMethod::End: return SEEK_END;
		}

		UC_ASSERT_ALWAYS_MSG("Invalid SeekMethod");
		return SEEK_SET;
	}
}

#endif