#include "TextDataLoader.hpp"
#include "unicore/File.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TextDataLoader::TextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> TextDataLoader::load(const ResourceLoaderContext& context)
	{
		context.file.seek(0);
		const auto size = context.file.size();

		String str;
		str.resize(size);
		if (context.file.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	WTextDataLoader::WTextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> WTextDataLoader::load(const ResourceLoaderContext& context)
	{
		context.file.seek(0);
		const auto size = context.file.size();

		String str;
		str.resize(size);
		if (context.file.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}
}