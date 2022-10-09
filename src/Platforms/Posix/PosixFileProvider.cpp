#include "PosixFileProvider.hpp"
#if defined(UNICORE_PLATFORM_POSIX)
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "unicore/Unicode.hpp"
#include "unicore/Logger.hpp"
#include "PosixError.hpp"
#include "PosixFile.hpp"

namespace unicore
{
	PosixFileProvider::PosixFileProvider(Logger& logger)
		: _logger(logger), _current_dir(get_current_dir())
	{
	}

	PosixFileProvider::PosixFileProvider(Logger& logger, const Path& current_dir)
		: _logger(logger), _current_dir(current_dir)
	{
	}

	bool PosixFileProvider::exists(const Path& path) const
	{
		const auto native_path = to_native(path);
		struct stat data;
		return stat(native_path.c_str(), &data) == 0;
	}

	Optional<FileStats> PosixFileProvider::stats(const Path& path) const
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

		const auto error = PosixError::get_last();
		if (!error.is_not_found())
			UC_LOG_ERROR(_logger) << error << L" for " << path;

		return std::nullopt;
	}

	uint16_t PosixFileProvider::enumerate_entries(
		const Path& path, StringView search_pattern,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		const auto native_path = to_native(path);

		UC_LOG_INFO(_logger) << "Enumerate for " << native_path;

		if (const auto dir = opendir(native_path.c_str()))
		{
			uint16_t count = 0;
			dirent* entry;
			while ((entry = readdir(dir)) != nullptr)
			{
				const auto file_name = StringView(entry->d_name);
				if (file_name == "." || file_name == "..")
					continue;

				const auto type = entry->d_type == DT_DIR ? FileType::Directory : FileType::File;
				if (!enumerate_test_options(type, options))
					continue;

				if (StringHelper::compare_to_mask(file_name, search_pattern))
				{
					count++;
					name_list.push_back(String(file_name));
				}
			}

			return count;
		}

		if (const auto error = PosixError::get_last(); !error.is_not_found())
		{
			UC_LOG_ERROR(_logger) << "Enumerate failed for "
				<< path << " with error " << error;
		}

		return 0;
	}

	bool PosixFileProvider::create_directory(const Path& path)
	{
		const auto native_path = to_native(path);

		if (mkdir(native_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
			return true;

		UC_LOG_ERROR(_logger) << "Failed to create dir at "
			<< path << " with error - " << PosixError::get_last();
		return false;
	}

	bool PosixFileProvider::delete_directory(const Path& path, bool recursive)
	{
		const auto native_path = to_native(path);
		if (rmdir(native_path.c_str()) == 0)
			return true;

		UC_LOG_ERROR(_logger) << "Failed to delete dir at "
			<< path << " with error - " << PosixError::get_last();
		return false;
	}

	Shared<ReadFile> PosixFileProvider::open_read(const Path& path)
	{
		const auto native_path = to_native(path);
		auto handle = fopen(native_path.c_str(), "rb");
		return handle ? make_shared<PosixFile>(handle) : nullptr;
	}

	Shared<WriteFile> PosixFileProvider::create_new(const Path& path)
	{
		const auto native_path = to_native(path);
		auto handle = fopen(native_path.c_str(), "wb");
		return handle ? make_shared<PosixFile>(handle) : nullptr;
	}

	bool PosixFileProvider::delete_file(const Path& path)
	{
		const auto native_path = to_native(path);
		if (remove(native_path.c_str()) == 0)
			return true;

		UC_LOG_ERROR(_logger) << "Failed to delete file at "
			<< path << " with error - " << PosixError::get_last();
		return false;
	}

	Path PosixFileProvider::get_current_dir() const
	{
#if defined(UNICORE_PLATFORM_EMSCRIPTEN)
		return Path(".");
#else
		return Path(get_current_dir_name());
#endif
	}

	String PosixFileProvider::to_native(const Path& path) const
	{
		return (_current_dir / path).native_path();
	}
}

#endif