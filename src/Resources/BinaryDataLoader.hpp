#pragma once
#include "unicore/BinaryData.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	class BinaryDataLoader : public ResourceLoaderT<BinaryData>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderT)
	public:
		BinaryDataLoader();
		UC_NODISCARD Shared<Resource> load(const Options& options) override;
	};
}