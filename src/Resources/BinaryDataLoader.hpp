#pragma once
#include "unicore/BinaryData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class BinaryDataLoader : public ResourceLoaderType<BinaryData>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderType)
	public:
		BinaryDataLoader();
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}