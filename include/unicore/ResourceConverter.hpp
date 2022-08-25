#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class ResourceCache;

	struct ResourceConverterContext
	{
		ResourceCache& cache;
		Logger* logger = nullptr;
	};

	class ResourceConverter : public virtual Object
	{
		UC_OBJECT(ResourceConverter, Object)
	public:
		UC_NODISCARD virtual TypeConstRef raw_type() const = 0;
		UC_NODISCARD virtual TypeConstRef resource_type() const = 0;

		UC_NODISCARD virtual Shared<Resource> convert(Resource& raw, const ResourceConverterContext& context) = 0;
	};

	template<typename TResource, typename TRawResource,
		std::enable_if_t<std::is_base_of_v<Resource, TResource>>* = nullptr,
		std::enable_if_t<std::is_base_of_v<Resource, TRawResource>>* = nullptr>
	class ResourceConverterT : public ResourceConverter
	{
		UC_OBJECT(ResourceConverterT, ResourceConverter)
	public:
		UC_NODISCARD TypeConstRef raw_type() const override { return get_type<TRawResource >(); }
		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<TResource>(); }

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
}