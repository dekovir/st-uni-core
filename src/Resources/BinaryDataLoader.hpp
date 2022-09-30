#pragma once
#include "unicore/BinaryData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct BinaryDataLoadPolicy : ResourceLoaderPolicy::ExtensionPolicy
	{
		BinaryDataLoadPolicy()
			: ExtensionPolicy({ L".dat" })
		{
		}
	};

	class BinaryDataLoader : public ResourceLoaderType<BinaryData, BinaryDataLoadPolicy>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderType)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}