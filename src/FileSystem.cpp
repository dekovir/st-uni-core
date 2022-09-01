#include "unicore/FileSystem.hpp"
#include "unicore/Logger.hpp"
#include "unicore/FileProviderLoader.hpp"

namespace unicore
{
	FileSystem::FileSystem(Logger& logger)
		: _logger(logger)
	{
	}

	void FileSystem::add_read(const Shared<ReadFileProvider>& provider)
	{
		_providers.insert(_providers.begin(), provider);
	}

	void FileSystem::set_write(const Shared<WriteFileProvider>& provider)
	{
		_write = provider;
	}

	void FileSystem::add_loader(const Shared<FileProviderLoader>& creator)
	{
		_provider_creators.insert(creator);
	}

	bool FileSystem::mount(const Path& path)
	{
		if (_providers.empty())
		{
			UC_LOG_ERROR(_logger) << "Failed to mount. No providers";
			return true;
		}

		for (auto it = _providers.rbegin(); it != _providers.rend(); ++it)
		{
			const auto provider = *it;

			auto stats = provider->stats(path);
			if (!stats.has_value()) continue;

			if (stats.value().flag == FileFlag::Directory)
			{
				add_read(std::make_shared<DirectoryFileProvider>(*provider, path));
				return true;
			}

			for (const auto& creator : _provider_creators)
			{
				if (!creator->can_load(path))
					continue;

				auto stream = provider->open_read(path);
				if (!stream) continue;

				if (const auto result = creator->load(stream); result)
				{
					add_read(result);
					return true;
				}

				UC_LOG_ERROR(_logger) << "Failed to create provider from "
					<< creator->type() << " with " << path;
			}
		}

		UC_LOG_WARNING(_logger) << "Failed to mount " << path;
		return false;
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

	uint16_t FileSystem::enumerate_entries(const Path& path,
		WStringView search_pattern, List<WString>& name_list, FileFlags flags) const
	{
		// TODO: Test this with multiple providers
		const auto size = name_list.size();

		Set<WString> names;
		for (const auto& provider : _providers)
		{
			provider->enumerate_entries(path, search_pattern, name_list, flags);
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