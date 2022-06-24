#pragma once
#include "unicore/StreamProvider.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "unicore/Logger.hpp"

namespace unicore
{
	class LinuxStreamProvider : public StreamProvider
	{
	public:
		explicit LinuxStreamProvider(Logger& logger);

		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;
		uint16_t enumerate(const Path& path,
			List<WString>& name_list, FileFlags flags) override;

		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;

	protected:
		Logger& _logger;

		static String to_native(const Path& path);
	};
}

#endif