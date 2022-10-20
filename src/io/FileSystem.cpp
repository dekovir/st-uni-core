#include "unicore/io/FileSystem.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	FileSystem::FileSystem(Logger& logger)
		: _logger(logger)
	{
	}

	void FileSystem::add_read(const Shared<ReadFileProvider>& provider)
	{
		// TODO: Test this
		_providers.insert(_providers.begin() + (_write ? 1 : 0), provider);
	}

	void FileSystem::set_write(const Shared<WriteFileProvider>& provider)
	{
		if (_write)
		{
			UC_LOG_ERROR(_logger) << "WriteFileProvider is already set";
			return;
		}

		_write = provider;
		_providers.insert(_providers.begin(), _write);
	}

	Optional<FileStats> FileSystem::stats(const Path& path) const
	{
		for (const auto& provider : _providers)
		{
			if (auto info = provider->stats(path); info.has_value())
				return info;
		}

		return std::nullopt;
	}

	uint16_t FileSystem::enumerate_entries(
		const Path& path, StringView search_pattern,
		List<String>& name_list, const EnumerateOptions& options) const
	{
		// TODO: Test this with multiple providers
		const auto size = name_list.size();

		Set<String> names;
		for (const auto& provider : _providers)
		{
			provider->enumerate_entries(path, search_pattern, name_list, options);
			names.insert(name_list.begin() + size, name_list.end());
			name_list.resize(size);
		}

		name_list.insert(name_list.end(), names.begin(), names.end());

		return names.size();
	}

	Shared<ReadFile> FileSystem::open_read(const Path& path)
	{
		for (const auto& provider : _providers)
		{
			if (auto file = provider->open_read(path))
				return file;
		}

		return nullptr;
	}

	bool FileSystem::create_directory(const Path& path)
	{
		return _write ? _write->create_directory(path) : false;
	}

	bool FileSystem::delete_directory(const Path& path, bool recursive)
	{
		return _write ? _write->delete_directory(path) : false;
	}

	Shared<WriteFile> FileSystem::create_new(const Path& path)
	{
		return _write ? _write->create_new(path) : nullptr;
	}

	bool FileSystem::delete_file(const Path& path)
	{
		return _write ? _write->delete_file(path) : false;
	}
}