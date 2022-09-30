#pragma once
#include "unicore/BinaryData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	struct BinaryDataLoadPolicy : ResourceLoaderPathPolicy::Extension
	{
		BinaryDataLoadPolicy()
			: Extension({ L".dat" })
		{
		}
	};

	class BinaryDataLoader : public ResourceLoaderTyped<BinaryData, BinaryDataLoadPolicy>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}