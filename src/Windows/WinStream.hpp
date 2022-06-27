#pragma once
#include "unicore/Stream.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "WinDefs.hpp"

namespace unicore
{
	class WinStream : public ReadStream, public WriteStream
	{
	public:
		WinStream(HANDLE handle);
		~WinStream() override;

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