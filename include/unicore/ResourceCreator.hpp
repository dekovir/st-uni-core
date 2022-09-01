#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	class ResourceCache;

	class ResourceCreator : public virtual Object
	{
		UC_OBJECT(ResourceCreator, Object)
	public:
		struct Options
		{
			ResourceCache& cache;
			Logger* logger = nullptr;
		};

		UC_NODISCARD virtual TypeConstRef resource_type() const = 0;
		UC_NODISCARD virtual int priority() const { return 0; }

		UC_NODISCARD virtual bool can_create(const std::any& value) const = 0;
		virtual Shared<Resource> create(const Options& options, const std::any& value) = 0;
	};

	// ResourceCreatorT ///////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceCreatorT : public ResourceCreator
	{
		UC_OBJECT(ResourceCreatorT, ResourceCreator)
	public:
		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<T>(); }

		Shared<Resource> create(const Options& options, const std::any& value) override
		{
			return create_typed(options, value);
		}

	protected:
		virtual Shared<T> create_typed(const Options& context, const std::any& value) = 0;
	};

	// ResourceCreatorData ////////////////////////////////////////////////////////
	template<typename T, typename TData,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceCreatorData : public ResourceCreatorT<T>
	{
		UC_OBJECT(ResourceCreatorData, ResourceCreator)
	public:
		UC_NODISCARD bool can_create(const std::any& value) const override
		{
			return convert_data(value) != nullptr;
		}

	protected:
		static const TData* convert_data(const std::any& value)
		{
			if (value.type() == typeid(TData))
			{
				static TData data;
				data = std::any_cast<TData>(value);
				return &data;
			}

			return nullptr;
		}

		Shared<T> create_typed(const typename ResourceCreatorT<T>::Options& options, const std::any& value) override
		{
			if (auto data = convert_data(value))
				return create_from_data(options, *data);

			return nullptr;
		}

		virtual Shared<T> create_from_data(const typename ResourceCreatorT<T>::Options& context, const TData& data) = 0;
	};
}