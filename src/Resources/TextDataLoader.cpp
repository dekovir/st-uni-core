#include "TextDataLoader.hpp"
#include "unicore/File.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	TextDataLoader::TextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> TextDataLoader::load(const Context& options)
	{
		options.file.seek(0);
		const auto size = options.file.size();

		String str;
		str.resize(size);
		if (options.file.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	WTextDataLoader::WTextDataLoader()
		: ResourceLoaderT({ L".txt" })
	{
	}

	Shared<Resource> WTextDataLoader::load(const Context& options)
	{
		options.file.seek(0);
		const auto size = options.file.size();

		String str;
		str.resize(size);
		if (options.file.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}
}