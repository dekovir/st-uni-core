#include "TextDataLoader.hpp"
#include "unicore/File.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> TextDataLoader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.open_read(context.path);
		if (!file) return nullptr;

		file->seek(0);
		const auto size = file->size();

		String str;
		str.resize(size);
		if (file->read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	Shared<Resource> WTextDataLoader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.open_read(context.path);
		if (!file) return nullptr;

		file->seek(0);
		const auto size = file->size();

		String str;
		str.resize(size);
		if (file->read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}
}