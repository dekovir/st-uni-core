#include "unicore/FileSystem.hpp"

namespace unicore
{
	bool FileSystem::exists(const Path& path) const
	{
		return std::any_of(_providers.begin(), _providers.end(),
			[&path](const StreamProvider* provider) { return provider->exists(path); });
	}

	Optional<FileStats> FileSystem::stats(const Path& path) const
	{
		for (const auto& provider : _providers)
		{
			if (auto value = provider->stats(path); value.has_value())
				return value.value();
		}

		return std::nullopt;
	}

	uint16_t FileSystem::enumerate(const Path& path,
		List<WString>& name_list, FileFlags flags)
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
			if (auto stream = provider->open_read(path); stream != nullptr)
				return stream;
		}

		return nullptr;
	}

	Shared<WriteStream> FileSystem::create_new(const Path& path)
	{
		for (const auto& provider : _providers)
		{
			if (auto stream = provider->create_new(path); stream != nullptr)
				return stream;
		}

		return nullptr;
	}

	void FileSystem::add_provider(StreamProvider& provider)
	{
		_providers.push_back(&provider);
	}
}