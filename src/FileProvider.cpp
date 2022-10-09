#include "unicore/FileProvider.hpp"
#include "unicore/Memory.hpp"
#include "unicore/File.hpp"
#include "unicore/StringHelper.hpp"

namespace unicore
{
	constexpr StringView Wildcard = "*";

	// FileProvider ///////////////////////////////////////////////////////////////
	size_t FileProvider::get_system_memory_use() const
	{
		return sizeof(FileProvider);
	}

	bool FileProvider::exists(const Path& path) const
	{
		return stats(path).has_value();
	}

	bool FileProvider::is_file(const Path& path) const
	{
		const auto info = stats(path);
		return info.has_value() && info.value().type == FileType::File;
	}

	bool FileProvider::is_directory(const Path& path) const
	{
		const auto info = stats(path);
		return info.has_value() && info.value().type == FileType::Directory;
	}

	uint16_t FileProvider::enumerate_entries(const Path& path,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		return enumerate_entries(path, Wildcard, name_list, options);
	}

	uint16_t FileProvider::enumerate_files(const Path& path,
		StringView search_pattern, List<String>& name_list) const
	{
		return enumerate_entries(path, search_pattern, name_list, { EnumerateFlag::Files });
	}

	uint16_t FileProvider::enumerate_files(const Path& path, List<String>& name_list) const
	{
		return enumerate_files(path, Wildcard, name_list);
	}

	uint16_t FileProvider::enumerate_dirs(const Path& path,
		StringView search_pattern, List<String>& name_list) const
	{
		return enumerate_entries(path, search_pattern, name_list, { EnumerateFlag::Directories });
	}

	uint16_t FileProvider::enumerate_dirs(const Path& path, List<String>& name_list) const
	{
		return enumerate_dirs(path, Wildcard, name_list);
	}

	List<String> FileProvider::get_files(const Path& path, StringView search_pattern) const
	{
		List<String> name_list;
		enumerate_files(path, search_pattern, name_list);
		return name_list;
	}

	List<String> FileProvider::get_files(const Path& path) const
	{
		List<String> name_list;
		enumerate_files(path, Wildcard, name_list);
		return name_list;
	}

	List<String> FileProvider::get_dirs(const Path& path, StringView search_pattern) const
	{
		List<String> name_list;
		enumerate_dirs(path, search_pattern, name_list);
		return name_list;
	}

	List<String> FileProvider::get_dirs(const Path& path) const
	{
		List<String> name_list;
		enumerate_dirs(path, Wildcard, name_list);
		return name_list;
	}

	// ReadFileProvider ///////////////////////////////////////////////////////////
	Shared<MemoryChunk> ReadFileProvider::read_chunk(const Path& path)
	{
		if (const auto stream = open_read(path))
		{
			auto size = stream->size();
			if (auto chunk = std::make_shared<MemoryChunk>(size); stream->read(*chunk))
				return chunk;
		}

		return nullptr;
	}

	bool ReadFileProvider::enumerate_test_flags(FileType type, EnumerateFlags flags)
	{
		switch (type)
		{
		case FileType::File: return flags.has(EnumerateFlag::Files);
		case FileType::Directory: return flags.has(EnumerateFlag::Directories);
		case FileType::Unknown: return false;
		}

		UC_ASSERT_ALWAYS_MSG("Invalid type");
		return false;
	}

	bool ReadFileProvider::enumerate_test_options(FileType type, const EnumerateOptions& options)
	{
		return enumerate_test_flags(type, options.flags);
	}

	// WriteFileProvider //////////////////////////////////////////////////////////
	bool WriteFileProvider::write_chunk(const Path& path, const MemoryChunk& chunk)
	{
		if (const auto stream = create_new(path))
			return stream->write(chunk);

		return false;
	}

	// DirectoryFileProvider //////////////////////////////////////////////////////
	size_t DirectoryFileProvider::get_system_memory_use() const
	{
		return sizeof(DirectoryFileProvider);
	}

	Optional<FileStats> DirectoryFileProvider::stats(const Path& path) const
	{
		return _provider.stats(make_path(path));
	}

	bool DirectoryFileProvider::exists(const Path& path) const
	{
		return _provider.exists(make_path(path));
	}

	uint16_t DirectoryFileProvider::enumerate_entries(
		const Path& path, StringView search_pattern,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		return _provider.enumerate_entries(make_path(path), search_pattern, name_list, options);
	}

	Shared<ReadFile> DirectoryFileProvider::open_read(const Path& path)
	{
		return _provider.open_read(make_path(path));
	}

	// CachedFileProvider /////////////////////////////////////////////////////////
	size_t CachedFileProvider::get_system_memory_use() const
	{
		return sizeof(CachedFileProvider);
	}

	bool CachedFileProvider::exists(const Path& path) const
	{
		return contains(path);
	}

	Optional<FileStats> CachedFileProvider::stats(const Path& path) const
	{
		const auto data = find_data(path);
		return data.has_value() ? stats_index(data.value()) : std::nullopt;
	}

	uint16_t CachedFileProvider::enumerate_entries(
		const Path& path, StringView search_pattern,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		uint16_t count = 0;
		for (const auto& [entry_path, entry_index] : _entries)
		{
			if (entry_path.starts_with(path))
			{
				// TODO: Optimize enumeration
				auto parent_path = entry_path.parent_path();
				if (path == parent_path)
				{
					if (enumerate_index(entry_index, options))
					{
						name_list.push_back(entry_path.filename());
						count++;
					}
				}
			}
		}

		return count;
	}

	Shared<ReadFile> CachedFileProvider::open_read(const Path& path)
	{
		const auto data = find_data(path);
		return data.has_value() ? open_read_index(data.value()) : nullptr;
	}

	bool CachedFileProvider::enumerate_index(intptr_t index, const EnumerateOptions& options) const
	{
		if (const auto stats = stats_index(index); stats.has_value())
		{
			const auto type = stats.value().type;
			return enumerate_test_options(type, options);
		}

		return false;
	}
}