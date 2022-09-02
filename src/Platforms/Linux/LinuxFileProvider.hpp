#pragma once
#include "unicore/FileProvider.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include "unicore/Logger.hpp"

namespace unicore
{
	class LinuxFileProvider : public WriteFileProvider
	{
		UC_OBJECT(LinuxFileProvider, WriteFileProvider)
	public:
		explicit LinuxFileProvider(Logger& logger);

		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path,
			WStringView search_pattern, List<WString>& name_list,
			const EnumerateOptions& options) const override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<ReadFile> open_read(const Path& path) override;
		Shared<WriteFile> create_new(const Path& path) override;

		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;

		static String to_native(const Path& path);
	};
}

#endif