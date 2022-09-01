#pragma once
#include "unicore/FileProvider.hpp"

namespace unicore
{
	class FileProviderLoader : public Object
	{
		UC_OBJECT(FileProviderLoader, Object)
	public:
		UC_NODISCARD virtual bool can_load(const Path& path) const = 0;
		virtual Shared<ReadFileProvider> load(const Shared<ReadFile>& stream) = 0;
	};
}