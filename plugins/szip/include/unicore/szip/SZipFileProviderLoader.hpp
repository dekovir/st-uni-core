#pragma once
#include "unicore/resource/ResourceLoader.hpp"
#include "unicore/szip/SZipFileProvider.hpp"

namespace unicore
{
	struct SZipPathPolicy : ResourceLoaderPathPolicy::Extension
	{
		explicit SZipPathPolicy()
			: Extension({ ".7z" })
		{
		}
	};

	class SZipFileProviderLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<ReadFileProvider>, SZipPathPolicy>
	{
		UC_OBJECT(SZipFileProviderLoader, ResourceLoader)
	public:
		SZipFileProviderLoader();
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}