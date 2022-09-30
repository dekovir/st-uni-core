#pragma once
#include "unicore/Path.hpp"
#include "unicore/Resource.hpp"
#include "unicore/File.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	class ResourceCache;

	class ResourceLoader : public Object
	{
		UC_OBJECT(ResourceLoader, Object)
	public:
		struct Context
		{
			ResourceCache& cache;
			Path path;
			const ResourceOptions* options;
			Logger* logger = nullptr;
		};

		UC_NODISCARD virtual TypeConstRef resource_type() const = 0;

		UC_NODISCARD virtual int priority() const { return 0; }
		UC_NODISCARD virtual const TypeInfo* options_type() const { return nullptr; }

		UC_NODISCARD virtual bool can_load(const Path& path) const = 0;
		UC_NODISCARD virtual bool can_load(const ResourceOptions* options) const = 0;

		UC_NODISCARD virtual Shared<Resource> load(const Context& context) = 0;
	};

	// ResourceLoaderPolicy ///////////////////////////////////////////////////////
	namespace ResourceLoaderPathPolicy
	{
		struct Empty
		{
			bool operator()(const Path& path) const { return path.empty(); }
		};

		struct NotEmpty
		{
			bool operator()(const Path& path) const { return !path.empty(); }
		};

		struct Extension
		{
			const Set<WStringView> extension;

			explicit Extension(const std::initializer_list<WStringView> extension_)
				: extension(extension_) {}

			bool operator()(const Path& path) const
			{
				return extension.find(path.extension()) != extension.end();
			}
		};
	}

	namespace ResourceLoaderOptionsPolicy
	{
		struct Null
		{
			bool operator()(const ResourceOptions* options) const { return options == nullptr; }
		};

		template<typename T, std::enable_if_t<std::is_base_of_v<ResourceOptions, T>>* = nullptr>
		struct Exact
		{
			bool operator()(const ResourceOptions* options) const
			{
				return options != nullptr && typeid(*options) == typeid(T);
			}
		};

		template<typename T, std::enable_if_t<std::is_base_of_v<ResourceOptions, T>>* = nullptr>
		struct NullOrExact : Exact<T>
		{
			bool operator()(const ResourceOptions* options) const
			{
				return options == nullptr || Exact<T>::operator()(options);
			}
		};
	}

	// ResourceLoaderTyped ////////////////////////////////////////////////////////
	template<typename T, typename PathPolicy,
		typename OptionsPolicy = ResourceLoaderOptionsPolicy::Null,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceLoaderTyped : public ResourceLoader
	{
		UC_OBJECT(ResourceLoaderTyped, ResourceLoader)
	public:
		UC_NODISCARD bool can_load(const Path& path) const override
		{
			return _path_policy(path);
		}

		UC_NODISCARD bool can_load(const ResourceOptions* options) const override
		{
			static const OptionsPolicy policy;
			return _options_policy(options);
		}

		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<T>(); }

	protected:
		const PathPolicy _path_policy;
		const OptionsPolicy _options_policy;
	};

	// ResourceLoaderOptionsTyped /////////////////////////////////////////////////
	template<typename T, typename TOptions, typename PathPolicy,
		typename OptionsPolicy = ResourceLoaderOptionsPolicy::Exact<TOptions>,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr,
		std::enable_if_t<std::is_base_of_v<ResourceOptions, TOptions>>* = nullptr>
	class ResourceLoaderOptionsTyped : public ResourceLoaderTyped<T, PathPolicy, OptionsPolicy>
	{
		UC_OBJECT(ResourceLoaderOptionsTyped, ResourceLoader)
	public:
		UC_NODISCARD const TypeInfo* options_type() const override { return &typeid(TOptions); }

		UC_NODISCARD bool can_load(const ResourceOptions* options) const override
		{
			return options == nullptr || dynamic_cast<const TOptions*>(options) != nullptr;
		}

		UC_NODISCARD Shared<Resource> load(const ResourceLoader::Context& context) override
		{
			if (context.options != nullptr)
			{
				auto options = dynamic_cast<const TOptions*>(context.options);
				if (options != nullptr)
					return load_options(context, *options);

				UC_LOG_ERROR(context.logger) << "Failed to cast options";
			}

			TOptions default_options{};
			return load_options(context, default_options);
		}

		UC_NODISCARD virtual Shared<Resource> load_options(
			const ResourceLoader::Context& context, const TOptions& options) = 0;
	};
}