#include "unicore/FileSystem.hpp"

namespace unicore
{
	bool FileSystem::exists(const Path& path) const
	{
		for (const auto& provider : _providers)
		{
			if (provider->exists(path))
				return true;
		}

		return false;
	}

	Optional<FileStats> FileSystem::stats(const Path& path) const
	{
		for (const auto& provider : _providers)
		{
			auto value = provider->stats(path);
			if (value.has_value())
				return value.value();
		}

		return std::nullopt;
	}

	uint16_t FileSystem::enumerate(const Path& path, List<Path>& name_list, FileFlags flags)
	{
		uint16_t count = 0;
		for (const auto& provider : _providers)
			count += provider->enumerate(path, name_list, flags);
		return count;
	}

	Shared<ReadStream> FileSystem::open_read(const Path& path)
	{
		for (const auto& provider : _providers)
		{
			auto stream = provider->open_read(path);
			if (stream != nullptr)
				return stream;
		}

		return nullptr;
	}

	Shared<WriteStream> FileSystem::create_new(const Path& path)
	{
		for (const auto& provider : _providers)
		{
			auto stream = provider->create_new(path);
			if (stream != nullptr)
				return stream;
		}

		return nullptr;
	}

	void FileSystem::add_provider(const Shared<StreamProvider>& provider)
	{
		_providers.insert(_providers.begin(), provider);
	}
}