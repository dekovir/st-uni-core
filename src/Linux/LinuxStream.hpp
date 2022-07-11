#pragma once
#include "unicore/Stream.hpp"
#if defined(UNICORE_PLATFORM_LINUX)

namespace unicore
{
	// TODO: Remove multiple inheritance
	class LinuxStream : public ReadStream, public WriteStream
	{
		UC_OBJECT(LinuxStream, ReadStream)
	public:
		explicit LinuxStream(FILE* handle);
		~LinuxStream() override;

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

		bool flush() override;
		bool write(const void* buffer, size_t size, size_t* bytes_written) override;

	protected:
		FILE* _handle;

		static int convert_method(SeekMethod method);
	};
}

#endif