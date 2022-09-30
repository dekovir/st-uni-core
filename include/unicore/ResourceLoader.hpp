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

		//protected:
		//	template<typename T,
		//		std::enable_if_t<std::is_base_of_v<ResourceOptions, T>>* = nullptr>
		//	static auto convert_options(const ResourceOptions* options)
		//	{
		//		return dynamic_cast<const T*>(options);
		//	}
	};

	// ResourceLoaderType /////////////////////////////////////////////////////////
	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceLoaderType : public ResourceLoader
	{
		UC_OBJECT(ResourceLoaderType, ResourceLoader)
	public:
		ResourceLoaderType() = default;
		ResourceLoaderType(const std::initializer_list<WStringView> ext_list)
			: _extensions(ext_list)
		{
		}

		UC_NODISCARD bool can_load(const Path& path) const override
		{
			return
				_extensions.empty() ||
				(_extensions.find(path.extension()) != _extensions.end());
		}

		UC_NODISCARD bool can_load(const ResourceOptions* options) const override { return options == nullptr; }

		UC_NODISCARD TypeConstRef resource_type() const override { return get_type<T>(); }

	private:
		const Set<WStringView> _extensions;
	};

	// ResourceLoaderTypeOptions //////////////////////////////////////////////////
	template<typename T, typename TOptions,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr,
		std::enable_if_t<std::is_base_of_v<ResourceOptions, TOptions>>* = nullptr>
	class ResourceLoaderTypeOptions : public ResourceLoaderType<T>
	{
		UC_OBJECT(ResourceLoaderTypeOptions, ResourceLoaderType<T>)
	public:
		ResourceLoaderTypeOptions() = default;
		ResourceLoaderTypeOptions(const std::initializer_list<WStringView> ext_list)
			: ParentType(ext_list)
		{}

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