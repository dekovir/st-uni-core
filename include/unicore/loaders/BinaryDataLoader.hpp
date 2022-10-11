#pragma once
#include "unicore/BinaryData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class BinaryDataLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Single<BinaryData>,
		ResourceLoaderPathPolicy::NotEmpty>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderTyped)
	public:
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}