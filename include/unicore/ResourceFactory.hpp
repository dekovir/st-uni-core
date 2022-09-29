#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	class ResourceFactory : public Resource
	{
		UC_OBJECT(ResourceFactory, Resource)
	public:
		struct Context
		{
			const ResourceOptions* options;
			Logger* logger;
		};

		UC_NODISCARD virtual TypeConstRef resource_type() const = 0;
		UC_NODISCARD virtual const TypeInfo* data_type() const = 0;
		UC_NODISCARD virtual Shared<Resource> create(const Context& context) = 0;
	};

	template<typename TResource, typename TOptions>
	class ResourceFactoryT : public ResourceFactory
	{
	public:
		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<TResource>(); }

		UC_NODISCARD const TypeInfo* data_type() const override { return &typeid(TOptions); }

		UC_NODISCARD Shared<Resource> create(const Context& context) override
		{
			if (context.options != nullptr)
			{
				auto data = dynamic_cast<const TOptions*>(context.options);
				if (data == nullptr)
					return nullptr;

				return create_options(context, *data);
			}

			return create_default({ nullptr, context.logger });
		}

		UC_NODISCARD virtual Shared<TResource> create_default(const Context& context)
		{
			return create_options(context, {});
		}

		UC_NODISCARD virtual Shared<TResource> create_options(const Context& context, const TOptions& options) = 0;
	};
}