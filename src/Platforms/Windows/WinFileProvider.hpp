#pragma once
#include "unicore/FileProvider.hpp"
#if defined(UNICORE_PLATFORM_WINDOWS)
#include "unicore/Logger.hpp"
#include "WinDefs.hpp"

namespace unicore
{
	class WinFileProvider : public WriteFileProvider
	{
		UC_OBJECT(WinFileProvider, WriteFileProvider)
	public:
		explicit WinFileProvider(Logger& logger);

		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path,
			WStringView search_pattern, List<WString>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<ReadFile> open_read(const Path& path) override;
		Shared<WriteFile> create_new(const Path& path) override;

		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;

		static DateTime to_datetime(FILETIME const& ft);
	};
}
#endif