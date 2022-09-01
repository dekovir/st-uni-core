#include "unicore/FileProvider.hpp"
#include "unicore/Memory.hpp"
#include "unicore/File.hpp"
#include "unicore/StringHelper.hpp"

namespace unicore
{
	// FileProvider ///////////////////////////////////////////////////////////////
	bool FileProvider::exists(const Path& path) const
	{
		return stats(path).has_value();
	}

	List<Path> FileProvider::enumerate(const Path& path,
		WStringView search_pattern, FileFlags flags) const
	{
		List<Path> name_list;
		enumerate(path, search_pattern, name_list, flags);
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

	uint16_t DirectoryFileProvider::enumerate(const Path& path,
		WStringView search_pattern, List<Path>& name_list, FileFlags flags) const
	{
		return _provider.enumerate(make_path(path), search_pattern, name_list, flags);
	}

	Shared<ReadFile> DirectoryFileProvider::open_read(const Path& path)
	{
		return _provider.open_read(make_path(path));
	}

	// ArchiveFileProvider ////////////////////////////////////////////////////////
	bool ArchiveFileProvider::exists(const Path& path) const
	{
		return find_data(path) != nullptr;
	}

	Optional<FileStats> ArchiveFileProvider::stats(const Path& path) const
	{
		const auto data = find_data(path);
		return data != nullptr ? stats_index(*data) : std::nullopt;
	}

	uint16_t ArchiveFileProvider::enumerate(
		const Path& path, WStringView search_pattern,
		List<Path>& name_list, FileFlags flags) const
	{
		uint16_t count = 0;
		if (const auto entry = find_entry(path); entry != nullptr)
		{
			for (const auto & it : entry->files)
			{
				if (StringHelper::compare_to_mask(WStringView(it.first), search_pattern))
				{
					if (!enumerate_index(it.second, flags))
						continue;

					name_list.push_back(path / it.first);
					count++;
				}
			}
		}

		return count;
	}

	Shared<ReadFile> ArchiveFileProvider::open_read(const Path& path)
	{
		const auto data = find_data(path);
		return data != nullptr ? open_read_index(*data) : nullptr;
	}
}