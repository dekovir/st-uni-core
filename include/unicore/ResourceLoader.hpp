#pragma once
#include "unicore/Path.hpp"
#include "unicore/Resource.hpp"
#include "unicore/Stream.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class ResourceCache;

	// ResourceLoader ////////////////////////////////////////////////////////////
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
		UC_NODISCARD virtual const List<WStringView>& extension() const = 0;

		UC_NODISCARD virtual Shared<Resource> load(const ResourceLoaderContext& context) = 0;
	};

	template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceLoaderT : public ResourceLoader
	{
	public:
		ResourceLoaderT(const std::initializer_list<WStringView> ext_list)
			: _extensions(ext_list)
		{
		}

		UC_NODISCARD TypeIndex resource_type() const override { return get_type_index<T>(); }
		UC_NODISCARD const List<WStringView>& extension() const override { return _extensions; }

	private:
		const List<WStringView> _extensions;
	};

	// ResourceConverter //////////////////////////////////////////////////////////
	struct ResourceConverterContext
	{
		ResourceCache& cache;
		Logger* logger = nullptr;
	};

	class ResourceConverter : public virtual Object
	{
	public:
		UC_NODISCARD virtual TypeIndex raw_type() const = 0;
		UC_NODISCARD virtual TypeIndex resource_type() const = 0;

		UC_NODISCARD virtual Shared<Resource> convert(Resource& raw, const ResourceConverterContext& context) = 0;
	};

	template<typename TResource, typename TRawResource,
		std::enable_if_t<std::is_base_of_v<Resource, TResource>>* = nullptr,
		std::enable_if_t<std::is_base_of_v<Resource, TRawResource>>* = nullptr>
	class ResourceConverterT : public ResourceConverter
	{
	public:
		UC_NODISCARD TypeIndex raw_type() const override { return get_type_index<TRawResource >(); }
		UC_NODISCARD TypeIndex resource_type() const override { return get_type_index<TResource>(); }

		Shared<Resource> convert(Resource& raw, const ResourceConverterContext& context) override
		{
			if (auto raw_resource = dynamic_cast<TRawResource*>(&raw))
				return convert_typed(*raw_resource, context);

			UC_LOG_ERROR(context.logger) << "Failed to cast resource";
			return nullptr;
		}

	protected:
		virtual Shared<TResource> convert_typed(TRawResource& raw, const ResourceConverterContext& context) = 0;
	};

	// ResourceCreator ///////////////////////////////////////////////////////////
	struct ResourceCreatorContext
	{
		ResourceCache& cache;
		Logger* logger = nullptr;
	};

	// TODO: Replace const void* with something (Variant?)
	using ResourceCreatorOptions = Dictionary<StringView, const void*>;

	class ResourceCreator : public virtual Object
	{
	public:
		virtual Shared<Resource> create(const ResourceCreatorContext& context, const ResourceCreatorOptions& options) = 0;
	};
}