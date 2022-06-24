#pragma once
#include "unicore/Stream.hpp"
#if defined(UNICORE_PLATFORM_LINUX)

namespace unicore
{
	class LinuxStream : public ReadStream, public WriteStream
	{
	public:
		explicit LinuxStream(FILE* handle);
		~LinuxStream() override;

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		size_t read(void* buffer, size_t size, size_t count) override;

		size_t write(const void* buffer, size_t size, size_t count) override;
		bool flush() override;

	protected:
		FILE* _handle;

		static int convert_method(SeekMethod method);
	};
}

#endif