#include "unicore/ResourceCache.hpp"
#include "unicore/LogHelper.hpp"

namespace unicore
{
	void ResourceCache::add_provider(const Shared<BasicStreamProvider>& provider)
	{
		_providers.push_back(provider);
	}

	void ResourceCache::add_loader(const Shared<ResourceLoader>& loader)
	{
		_loaders_dict[loader->resource_type()].push_back(loader);
	}

	const List<Shared<ResourceLoader>>& ResourceCache::get_loaders(TypeIndex index) const
	{
		static List<Shared<ResourceLoader>> s_empty;
		const auto it = _loaders_dict.find(index);
		return it != _loaders_dict.end() ? it->second : s_empty;
	}

	Shared<Resource> ResourceCache::find(const Path& path, TypeIndex type) const
	{
		const auto find_it = _cached.find(path);
		if (find_it == _cached.end()) return nullptr;

		for (const auto& it : find_it->second)
		{
			if (it.first == type) return it.second;
		}

		return nullptr;
	}

	Shared<Resource> ResourceCache::load(const Path& path, TypeIndex type)
	{
		if (auto resource_find = find(path, type))
			return resource_find;

		auto& loaders = get_loaders(type);

		if (loaders.empty())
		{
			UC_LOG_WARNING(_logger) << "No loaders for " << type;
			return nullptr;
		}

		const auto extension = path.extension();

		for (const auto& provider : _providers)
		{
			auto stream = provider->open_read(path);

			ResourceLoaderContext context{ path, *this, *stream, _logger.get() };

			for (const auto& loader : loaders)
			{
				if (!loader->can_load_extension(extension))
					continue;

				if (auto resource = loader->load(context))
				{
					_cached[path][type] = resource;
					UC_LOG_DEBUG(_logger) << "Loaded '" << type << "' from " << path;
					return resource;
				}

				UC_LOG_ERROR(_logger) << "Can't load '" << type << "' from " << path;
			}
		}

		return nullptr;
	}
}