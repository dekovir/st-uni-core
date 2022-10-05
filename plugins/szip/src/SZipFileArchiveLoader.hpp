#pragma once
#include "unicore/ResourceLoader.hpp"
#include "unicore/szip/SZipFileProvider.hpp"

namespace unicore
{
	struct SZipPathPolicy : ResourceLoaderPathPolicy::Extension
	{
		explicit SZipPathPolicy()
			: Extension({ L".7z" })
		{
		}
	};

	class SZipFileArchiveLoader : public ResourceLoaderTyped<
		ResourceLoaderTypePolicy::Multiple<ReadFileProvider>, SZipPathPolicy>
	{
		UC_OBJECT(SZipFileArchiveLoader, ResourceLoader)
	public:
		SZipFileArchiveLoader();
		UC_NODISCARD Shared<Resource> load(const Context& context) override;
	};
}