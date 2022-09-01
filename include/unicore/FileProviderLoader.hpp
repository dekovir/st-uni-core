#pragma once
#include "unicore/FileProvider.hpp"

namespace unicore
{
	class FileProviderLoader : public Object
	{
		UC_OBJECT(FileProviderLoader, Object)
	public:
		struct Options
		{
			Logger* logger;
		};

		UC_NODISCARD virtual bool can_load(const Path& path) const = 0;
		virtual Shared<ReadFileProvider> load(const Shared<ReadFile>& stream, const Options& options) = 0;
	};
}