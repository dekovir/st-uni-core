#include "unicore/StringData.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	StringDataLoader::StringDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> StringDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto size = context.stream.size();
		context.stream.seek(0);
		String str;
		str.resize(size);
		if (context.stream.read(str.data(), size))
			return std::make_shared<StringData>(str);

		return nullptr;
	}
}