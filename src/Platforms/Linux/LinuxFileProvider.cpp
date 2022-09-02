#include "LinuxFileProvider.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include <sys/stat.h>
#include "unicore/Unicode.hpp"
#include "unicore/Logger.hpp"
#include "LinuxError.hpp"
#include "LinuxFile.hpp"

namespace unicore
{
	LinuxFileProvider::LinuxFileProvider(Logger& logger)
		: _logger(logger)
	{
	}

	bool LinuxFileProvider::exists(const Path& path) const
	{
		const auto native_path = to_native(path);
		struct stat data;
		return stat(native_path.c_str(), &data) == 0;
	}

	Optional<FileStats> LinuxFileProvider::stats(const Path& path) const
	{
		const auto native_path = to_native(path);

		struct stat data;
		if (stat(native_path.c_str(), &data) == 0)
		{
			FileStats stats;
			stats.size = data.st_size;
			stats.access_time = DateTime(data.st_atime);
			stats.mod_time = DateTime(data.st_mtime);
			stats.create_time = DateTime(data.st_ctime);
			stats.type = (data.st_mode & S_IFMT) == S_IFDIR
				? FileType::Directory : FileType::File;
			return stats;
		}

		const auto error = LinuxError::get_last();
		if (!error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for " << path;

		return std::nullopt;
	}

	uint16_t LinuxFileProvider::enumerate_entries(
		const Path& path, WStringView search_pattern,
		List<WString>& name_list, const EnumerateOptions& options) const
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented");
		return 0;
	}

	bool LinuxFileProvider::create_directory(const Path& path)
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented");
		return false;
	}

	bool LinuxFileProvider::delete_directory(const Path& path, bool recursive)
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented");
		return false;
	}

	Shared<ReadFile> LinuxFileProvider::open_read(const Path& path)
	{
		const auto native_path = to_native(path);
		auto handle = fopen(native_path.c_str(), "rb");
		return handle ? make_shared<LinuxFile>(handle) : nullptr;
	}

	Shared<WriteFile> LinuxFileProvider::create_new(const Path& path)
	{
		const auto native_path = to_native(path);
		auto handle = fopen(native_path.c_str(), "wb");
		return handle ? make_shared<LinuxFile>(handle) : nullptr;
	}

	bool LinuxFileProvider::delete_file(const Path& path)
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented");
		return false;
	}

	String LinuxFileProvider::to_native(const Path& path)
	{
		return Unicode::to_utf8(path.native_path());
	}
}

#endif