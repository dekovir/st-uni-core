#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	class ResourceSaver : public virtual ResourceManipulator
	{
	public:
		virtual ResourceManipulatorCode save(WriteStream& stream, Resource& resource, Logger* logger = nullptr) = 0;
	};

	template<typename T, std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceSaverT : public ResourceSaver
	{
	public:
		UC_NODISCARD [[nodiscard]] TypeIndex resource_type() const override { return get_type_index<T>(); }
		ResourceManipulatorCode save(WriteStream& stream, Resource& resource, Logger* logger) override
		{
			if (auto resource_typed = dynamic_cast<T*>(&resource))
				return save_typed(stream, resource_typed, logger);

			return ResourceManipulatorCode::FailedWrongFormat;
		}

	protected:
		ResourceManipulatorCode save_typed(WriteStream& stream, T& resource, Logger* logger) = 0;
	};
}
