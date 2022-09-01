#pragma once
#include "unicore/File.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinDefs.hpp"

namespace unicore
{
	// TODO: Remove multiple inheritance
	class WinFile : public WriteFile
	{
		UC_OBJECT(WinFile, ReadFile)
	public:
		explicit WinFile(HANDLE handle);
		~WinFile() override;

		WinFile(const WinFile& other) = delete;
		WinFile(WinFile&& other) noexcept;

		WinFile& operator=(const WinFile& other) = delete;
		WinFile& operator=(WinFile&& other) noexcept;

		UC_NODISCARD int64_t size() const override;
		int64_t seek(int64_t offset, SeekMethod method) override;

		UC_NODISCARD bool eof() const override;
		bool read(void* buffer, size_t size, size_t* bytes_read) override;

		bool flush() override;
		bool write(const void* buffer, size_t size, size_t* bytes_written) override;

	protected:
		HANDLE _handle;

		static DWORD convert_method(SeekMethod method);
	};
}
#endif