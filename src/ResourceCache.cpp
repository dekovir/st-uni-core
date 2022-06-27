#include "unicore/ResourceCache.hpp"
#include "unicore/LogHelper.hpp"
#include "unicore/RenderResource.hpp"

namespace unicore
{
	ResourceCache::ResourceCache(Logger& logger)
		: _logger(logger)
	{}

	void ResourceCache::unload_all()
	{
		_cached.clear();
	}

	void ResourceCache::clear()
	{
		_providers.clear();
	}

	void ResourceCache::add_provider(StreamProvider& provider)
	{
		_providers.push_back(&provider);
	}

	Shared<ReadStream> ResourceCache::open_read(const Path& path) const
	{
		for (const auto& provider : _providers)
		{
			if (auto stream = provider->open_read(path))
				return stream;
		}

		return nullptr;
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

		if (_context == nullptr)
		{
			UC_LOG_ERROR(_logger) << "ResourceCache module not registered";
			return nullptr;
		}

		const auto loaders = _context->get_loaders(type);
		if (loaders.empty())
		{
			UC_LOG_WARNING(_logger) << "Empty loaders for " << type;
			return nullptr;
		}

		const auto extension = path.extension();
		Shared<ReadStream> stream;

		for (const auto& loader : loaders)
		{
			// TODO: Implement loading stack for prevent recursive loading
			if (!loader->can_load_extension(extension))
				continue;

			if (!stream)
			{
				stream = open_read(path);
				if (!stream)
				{
					UC_LOG_ERROR(_logger) << "Failed to open file at " << path;
					return nullptr;
				}
			}

			if (auto resource = loader->load({ path, *this, *stream, &_logger }))
			{
				_cached[path][type] = resource;
				UC_LOG_DEBUG(_logger) << "Loaded " << type << " from " << path;
				return resource;
			}

			UC_LOG_ERROR(_logger) << "Can't load " << type << " from " << path;
		}

		return nullptr;
	}

	void ResourceCache::calc_memory_use(size_t* system, size_t* video) const
	{
		if (system == nullptr && video == nullptr) return;

		if (system != nullptr)
			*system = 0;

		if (video != nullptr)
			*video = 0;

		for (const auto& it : _cached)
		{
			for (const auto& [_, resource] : it.second)
			{
				if (system != nullptr)
					*system += resource->system_memory_use();
				//if (video != nullptr)
				//{
				//	if (const auto render_resource = std::dynamic_pointer_cast<RenderResource>(resource))
				//		*video += render_resource->video_memory_use();
				//}
			}
		}
	}

	void ResourceCache::register_module(Context& context)
	{
		Module::register_module(context);

		_context = &context;
	}

	void ResourceCache::unregister_module(Context& context)
	{
		_context = nullptr;
		Module::unregister_module(context);
	}
}