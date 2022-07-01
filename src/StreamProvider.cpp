#include "unicore/StreamProvider.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	bool StreamProvider::exists(const Path& path) const
	{
		return stats(path).has_value();
	}

	Shared<MemoryChunk> StreamProvider::read_chunk(const Path& path)
	{
		if (const auto stream = open_read(path))
		{
			auto size = stream->size();
			auto chunk = std::make_shared<MemoryChunk>(size);
			if (stream->read(chunk->data(), size))
				return chunk;
		}

		return nullptr;
	}

	bool StreamProvider::write_chunk(const Path& path, const MemoryChunk& chunk)
	{
		if (const auto stream = create_new(path))
			return stream->write(chunk.data(), chunk.size());

		return false;
	}

	Optional<FileStats> PathStreamProvider::stats(const Path& path) const
	{
		return _provider.stats(make_path(path));
	}

	bool PathStreamProvider::exists(const Path& path) const
	{
		return _provider.exists(make_path(path));
	}

	uint16_t PathStreamProvider::enumerate(const Path& path, List<WString>& name_list, FileFlags flags)
	{
		return _provider.enumerate(make_path(path), name_list, flags);
	}

	Shared<ReadStream> PathStreamProvider::open_read(const Path& path)
	{
		return _provider.open_read(make_path(path));
	}

	Shared<WriteStream> PathStreamProvider::create_new(const Path& path)
	{
		return _provider.create_new(make_path(path));
	}
}