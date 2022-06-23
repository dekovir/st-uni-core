#include "unicore/StreamProvider.hpp"

namespace unicore
{
	bool BasicStreamProvider::exists(const Path& path) const
	{
		return stats(path).has_value();
	}

	Optional<FileStats> PathStreamProvider::stats(const Path& path) const
	{
		return _provider.stats(make_path(path));
	}

	bool PathStreamProvider::exists(const Path& path) const
	{
		return _provider.exists(make_path(path));
	}

	uint16_t PathStreamProvider::enumerate(const Path& path, List<Path>& name_list, FileFlags flags)
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