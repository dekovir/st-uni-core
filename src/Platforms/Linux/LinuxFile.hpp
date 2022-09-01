#pragma once
#include "unicore/File.hpp"
#if defined(UNICORE_PLATFORM_LINUX)

namespace unicore
{
	// TODO: Remove multiple inheritance
	class LinuxFile : public WriteFile
	{
		UC_OBJECT(LinuxFile, ReadFile)
	public:
		explicit LinuxFile(FILE* handle);
		~LinuxFile() override;

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