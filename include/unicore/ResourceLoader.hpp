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
		UC_OBJECT(ResourceLoader, Object)
	public:
		UC_NODISCARD virtual int priority() const { return 0; }

		UC_NODISCARD virtual TypeConstRef resource_type() const = 0;
		UC_NODISCARD virtual const List<WStringView>& extension() const = 0;

		UC_NODISCARD virtual Shared<Resource> load(const ResourceLoaderContext& context) = 0;
	};

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceLoaderT : public ResourceLoader
	{
		UC_OBJECT(ResourceLoaderT, ResourceLoader)
	public:
		ResourceLoaderT(const std::initializer_list<WStringView> ext_list)
			: _extensions(ext_list)
		{
		}

		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<T>(); }
		UC_NODISCARD const List<WStringView>& extension() const override { return _extensions; }

	private:
		const List<WStringView> _extensions;
	};
}