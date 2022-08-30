#pragma once
#include "unicore/StreamProvider.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "unicore/Logger.hpp"

namespace unicore
{
	class LinuxStreamProvider : public WriteStreamProvider
	{
		UC_OBJECT(LinuxStreamProvider, WriteStreamProvider)
	public:
		explicit LinuxStreamProvider(Logger& logger);

		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<StreamStats> stats(const Path& path) const override;

		uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = StreamFlag::File | StreamFlag::Directory) const override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;

		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;

		static String to_native(const Path& path);
	};
}

#endif