#include "unicore/FileProvider.hpp"
#include "unicore/Memory.hpp"
#include "unicore/File.hpp"
#include "unicore/StringHelper.hpp"

namespace unicore
{
	constexpr WStringView Wildcard = L"*";

	// FileProvider ///////////////////////////////////////////////////////////////
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
		List<WString>& name_list, const EnumerateOptions& options) const
	{
		return enumerate_entries(path, Wildcard, name_list, options);
	}

	uint16_t FileProvider::enumerate_files(const Path& path,
		WStringView search_pattern, List<WString>& name_list) const
	{
		return enumerate_entries(path, search_pattern, name_list, { EnumerateFlag::AllFiles });
	}

	uint16_t FileProvider::enumerate_files(const Path& path, List<WString>& name_list) const
	{
		return enumerate_files(path, Wildcard, name_list);
	}

	uint16_t FileProvider::enumerate_dirs(const Path& path,
		WStringView search_pattern, List<WString>& name_list) const
	{
		return enumerate_entries(path, search_pattern, name_list, { EnumerateFlag::AllDirectories });
	}

	uint16_t FileProvider::enumerate_dirs(const Path& path, List<WString>& name_list) const
	{
		return enumerate_dirs(path, Wildcard, name_list);
	}

	List<WString> FileProvider::get_files(const Path& path, WStringView search_pattern) const
	{
		List<WString> name_list;
		enumerate_files(path, search_pattern, name_list);
		return name_list;
	}

	List<WString> FileProvider::get_files(const Path& path) const
	{
		List<WString> name_list;
		enumerate_files(path, Wildcard, name_list);
		return name_list;
	}

	List<WString> FileProvider::get_dirs(const Path& path, WStringView search_pattern) const
	{
		List<WString> name_list;
		enumerate_dirs(path, search_pattern, name_list);
		return name_list;
	}

	List<WString> FileProvider::get_dirs(const Path& path) const
	{
		List<WString> name_list;
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
		case FileType::File: return flags.has(EnumerateFlag::AllFiles);
		case FileType::Directory: return flags.has(EnumerateFlag::AllDirectories);
		case FileType::Unknown: return false;
		}

		UC_ASSERT_ALWAYS_MSG("Invalid type");
		return false;
	}

	// WriteFileProvider //////////////////////////////////////////////////////////
	bool WriteFileProvider::write_chunk(const Path& path, const MemoryChunk& chunk)
	{
		if (const auto stream = create_new(path))
			return stream->write(chunk);

		return false;
	}

	// DirectoryFileProvider //////////////////////////////////////////////////////
	Optional<FileStats> DirectoryFileProvider::stats(const Path& path) const
	{
		return _provider.stats(make_path(path));
	}

	bool DirectoryFileProvider::exists(const Path& path) const
	{
		return _provider.exists(make_path(path));
	}

	uint16_t DirectoryFileProvider::enumerate_entries(
		const Path& path, WStringView search_pattern,
		List<WString>& name_list, const EnumerateOptions& options) const
	{
		return _provider.enumerate_entries(make_path(path), search_pattern, name_list, options);
	}

	Shared<ReadFile> DirectoryFileProvider::open_read(const Path& path)
	{
		return _provider.open_read(make_path(path));
	}

	// CachedFileProvider /////////////////////////////////////////////////////////
	bool CachedFileProvider::exists(const Path& path) const
	{
		return find_data(path) != nullptr;
	}

	Optional<FileStats> CachedFileProvider::stats(const Path& path) const
	{
		const auto data = find_data(path);
		return data != nullptr ? stats_index(*data) : std::nullopt;
	}

	uint16_t CachedFileProvider::enumerate_entries(
		const Path& path, WStringView search_pattern,
		List<WString>& name_list, const EnumerateOptions& options) const
	{
		uint16_t count = 0;
		if (const auto entry = find_entry(path); entry != nullptr)
		{
			for (const auto& [name, index] : entry->files)
			{
				if (StringHelper::compare_to_mask(WStringView(name), search_pattern))
				{
					if (!enumerate_index(index, options))
						continue;

					name_list.push_back(name);
					count++;
				}
			}
		}

		return count;
	}

	Shared<ReadFile> CachedFileProvider::open_read(const Path& path)
	{
		const auto data = find_data(path);
		return data != nullptr ? open_read_index(*data) : nullptr;
	}
}