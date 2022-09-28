#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/xml/XMLData.hpp"

namespace unicore
{
	class XMLDataLoader : public ResourceLoaderT<XMLData>
	{
	public:
		XMLDataLoader();

		UC_NODISCARD Shared<Resource> load(const Context& options) override;
	};
}