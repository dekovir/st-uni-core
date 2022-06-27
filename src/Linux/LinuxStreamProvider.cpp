#include "LinuxStreamProvider.hpp"
#if defined(UNICORE_PLATFORM_LINUX)
#include <sys/stat.h>
#include "unicore/Strings.hpp"
#include "unicore/LogHelper.hpp"
#include "LinuxError.hpp"

namespace unicore
{
	LinuxStreamProvider::LinuxStreamProvider(Logger& logger)
		: _logger(logger)
	{
	}

	bool LinuxStreamProvider::exists(const Path& path) const
	{
		const auto native_path = to_native(path);
		struct stat data;
		return stat(native_path.c_str(), &data) == 0;
	}

	Optional<FileStats> LinuxStreamProvider::stats(const Path& path) const
	{
		const auto native_path = to_native(path);

		struct stat data;
		if (stat(native_path.c_str(), &data) == 0)
		{
			FileStats stats;
			stats.size = data.st_size;
			stats.accestime = data.st_atime;
			stats.modtime = data.st_mtime;
			stats.createtime = data.st_ctime;
			stats.flag = (data.st_mode & S_IFMT) == S_IFDIR
				? FileFlag::Directory
				: FileFlag::File;
			return stats;
		}

		const auto error = LinuxError::get_last();
		if (!error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for " << path;

		return std::nullopt;
	}

	uint16_t LinuxStreamProvider::enumerate(const Path& path,
		List<WString>& name_list, FileFlags flags)
	{
	}

	Shared<ReadStream> LinuxStreamProvider::open_read(const Path& path)
	{
		const auto native_path = to_native(path);

		auto handle = fopen(native_path.c_str(), "rb");
	}

	Shared<WriteStream> LinuxStreamProvider::create_new(const Path& path)
	{
	}

	String LinuxStreamProvider::to_native(const Path& path)
	{
		return Strings::to_utf8(path.native_path());
	}
}

#endif