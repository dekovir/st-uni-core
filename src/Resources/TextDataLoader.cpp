#include "TextDataLoader.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TextDataLoader::TextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> TextDataLoader::load(const ResourceLoaderContext& context)
	{
		context.stream.seek(0);
		const auto size = context.stream.size();

		String str;
		str.resize(size);
		if (context.stream.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	WTextDataLoader::WTextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> WTextDataLoader::load(const ResourceLoaderContext& context)
	{
		context.stream.seek(0);
		const auto size = context.stream.size();

		String str;
		str.resize(size);
		if (context.stream.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}
}