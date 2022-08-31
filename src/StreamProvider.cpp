#include "unicore/StreamProvider.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Stream.hpp"
#include "unicore/StringHelper.hpp"

namespace unicore
{
	// BasicStreamProvider ////////////////////////////////////////////////////////
	bool BasicStreamProvider::exists(const Path& path) const
	{
		return stats(path).has_value();
	}

	List<Path> BasicStreamProvider::enumerate(const Path& path,
		WStringView search_pattern, FileFlags flags) const
	{
		List<Path> name_list;
		enumerate(path, search_pattern, name_list, flags);
		return name_list;
	}

	// ReadStreamProvider /////////////////////////////////////////////////////////
	Shared<MemoryChunk> ReadStreamProvider::read_chunk(const Path& path)
	{
		if (const auto stream = open_read(path))
		{
			auto size = stream->size();
			if (auto chunk = std::make_shared<MemoryChunk>(size); stream->read(*chunk))
				return chunk;
		}

		return nullptr;
	}

	// WriteStreamProvider ////////////////////////////////////////////////////////
	bool WriteStreamProvider::write_chunk(const Path& path, const MemoryChunk& chunk)
	{
		if (const auto stream = create_new(path))
			return stream->write(chunk);

		return false;
	}

	// DirectoryStreamProvider ////////////////////////////////////////////////////
	Optional<StreamStats> DirectoryStreamProvider::stats(const Path& path) const
	{
		return _provider.stats(make_path(path));
	}

	bool DirectoryStreamProvider::exists(const Path& path) const
	{
		return _provider.exists(make_path(path));
	}

	uint16_t DirectoryStreamProvider::enumerate(const Path& path,
		WStringView search_pattern, List<Path>& name_list, FileFlags flags) const
	{
		return _provider.enumerate(make_path(path), search_pattern, name_list, flags);
	}

	Shared<ReadStream> DirectoryStreamProvider::open_read(const Path& path)
	{
		return _provider.open_read(make_path(path));
	}

	// ArchiveStreamProvider //////////////////////////////////////////////////////
	bool ArchiveStreamProvider::exists(const Path& path) const
	{
		return find_data(path) != nullptr;
	}

	Optional<StreamStats> ArchiveStreamProvider::stats(const Path& path) const
	{
		const auto data = find_data(path);
		return data != nullptr ? stats_index(*data) : std::nullopt;
	}

	uint16_t ArchiveStreamProvider::enumerate(
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

	Shared<ReadStream> ArchiveStreamProvider::open_read(const Path& path)
	{
		const auto data = find_data(path);
		return data != nullptr ? open_read_index(*data) : nullptr;
	}
}