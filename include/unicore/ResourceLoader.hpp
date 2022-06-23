#pragma once
#include "unicore/Path.hpp"
#include "unicore/Resource.hpp"
#include "unicore/Stream.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class ResourceCache;

	struct ResourceLoaderContext
	{
		Path path;
		ResourceCache& cache;
		ReadStream& stream;
		Logger* logger = nullptr;
	};

	class ResourceLoader : public virtual Object
	{
	public:
		UC_NODISCARD virtual TypeIndex resource_type() const = 0;
		UC_NODISCARD virtual bool can_load_extension(WStringView ext) const { return true; }
		UC_NODISCARD virtual Shared<Resource> load(const ResourceLoaderContext& context) = 0;
	};

	template<typename T, class = std::enable_if_t<std::is_base_of_v<Resource, T>>>
	class ResourceLoaderT : public ResourceLoader
	{
	public:
		UC_NODISCARD TypeIndex resource_type() const override { return get_type_index<T>(); }
	};
}