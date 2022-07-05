#include "unicore/ResourceCache.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Logger.hpp"
#include "unicore/RenderResource.hpp"

namespace unicore
{
	ResourceCache::ResourceCache(Logger& logger)
		: _logger(logger)
	{
	}

	void ResourceCache::unload_all()
	{
		_cached.clear();
	}

	void ResourceCache::unload_unused()
	{
		for (auto& [path, map] : _cached)
		{
			for (auto jt = map.begin(); jt != map.end();)
			{
				if (jt->second.use_count() == 1)
				{
					UC_LOG_DEBUG(_logger) << "Unload resource " << jt->first << " from " << path;
					jt = map.erase(jt);
				}
				else ++jt;
			}
		}
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
		if (!_providers.empty())
		{
			for (const auto& provider : _providers)
			{
				if (auto stream = provider->open_read(path))
					return stream;
			}
		}
		else
		{
			UC_LOG_WARNING(_logger) << "No providers";
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

	Shared<Resource> ResourceCache::load(const Path& path, TypeIndex type, ResourceCacheFlags flags)
	{
		if (auto resource_find = find(path, type))
			return resource_find;

		if (_context == nullptr)
		{
			UC_LOG_ERROR(_logger) << "ResourceCache module not registered";
			return nullptr;
		}

		const auto logger = !flags.has(ResourceCacheFlag::Quiet) ? &_logger : nullptr;

		if (const auto loaders = _context->get_loaders(type); !loaders.empty())
		{
			// TODO: Implement loading stack for prevent recursive loading
			for (const auto& loader : loaders)
			{
				if (path.has_extension(L".*"))
				{
					for (const auto& extension : loader->extension())
					{
						Path new_path(path);
						new_path.replace_extension(extension);

						const auto stream = open_read(new_path);
						if (!stream)
							continue;

						if (auto resource = load_resource(*loader, new_path, *stream, type, logger))
							return resource;
					}
				}
				else
				{
					const auto extension = path.extension();

					const auto stream = open_read(path);
					if (!stream)
					{
						UC_LOG_WARNING(logger) << "Failed to open " << path;
						return nullptr;
					}

					if (auto resource = load_resource(*loader, path, *stream, type, logger))
						return resource;
				}
			}
		}

		if (const auto converters = _context->get_converters(type); !converters.empty())
		{
			for (const auto& converter : converters)
			{
				// TODO: Handle cyclic loading
				if (auto raw = load(path, converter->raw_type(), flags))
				{
					if (auto resource = converter->convert(*raw, { *this, logger }))
					{
						add_resource(resource, path, type);
						return resource;
					}
				}
			}
		}

		UC_LOG_WARNING(logger) << "Failed to load " << type << " from " << path;
		return nullptr;
	}

	void ResourceCache::dump_used()
	{
		unsigned index = 0;
		MemorySize sys_mem{ 0 };

		UC_LOG_INFO(_logger) << "Used resource dump";
		UC_LOG_INFO(_logger) << "----------------------------------";
		for (const auto& [path, map] : _cached)
		{
			for (const auto& [type, resource] : map)
			{
				const auto memory_use = MemorySize{ resource->system_memory_use() };
				sys_mem += memory_use;
				UC_LOG_INFO(_logger) << index << ": " << path << " " << type
					<< " [" << resource.use_count()
					<< ", " << memory_use << "]";
				index++;
			}
		}

		UC_LOG_INFO(_logger) << "----------------------------------";
		UC_LOG_INFO(_logger) << "Used system memory: " << sys_mem;
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
				if (video != nullptr)
				{
					if (const auto render_resource = std::dynamic_pointer_cast<RenderResource>(resource))
						*video += render_resource->video_memory_use();
				}
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

	Shared<Resource> ResourceCache::load_resource(ResourceLoader& loader,
		const Path& path, ReadStream& stream, TypeIndex type, Logger* logger)
	{
		if (auto resource = loader.load({ path, *this, stream, logger }))
		{
			add_resource(resource, path, type);
			return resource;
		}

		return nullptr;
	}

	bool ResourceCache::add_resource(const Shared<Resource>& resource, const Path& path, TypeIndex type)
	{
		_cached[path][type] = resource;
		UC_LOG_DEBUG(_logger) << "Loaded " << type << " from " << path
			<< " " << MemorySize{ resource->system_memory_use() };
		return true;
	}
}