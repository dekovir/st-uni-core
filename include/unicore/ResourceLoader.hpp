#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	struct ResourceLoaderResult
	{
		ResourceManipulatorCode code;
		Shared<Resource> resource = nullptr;
	};

	class ResourceLoader : public virtual ResourceManipulator
	{
	public:
		UC_NODISCARD virtual bool can_load_extension(WStringView ext) const { return true; }
		UC_NODISCARD virtual ResourceLoaderResult load(ReadStream& stream, Logger* logger = nullptr) = 0;
	};

	template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceLoaderT : public ResourceLoader
	{
	public:
		UC_NODISCARD TypeIndex resource_type() const override { return get_type_index<T>(); }
	};
}
