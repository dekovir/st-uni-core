#include "unicore/loaders/TextDataLoader.hpp"
#include "unicore/io/File.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> TextDataLoader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.load<ReadFile>(context.path);
		if (!file) return nullptr;

		file->seek(0);
		const auto size = file->size();

		String str;
		str.resize(size);
		if (file->read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	Shared<Resource> TextData32Loader::load(const Context& context)
	{
		// TODO: Log open_read failed
		const auto file = context.cache.load<ReadFile>(context.path);
		if (!file) return nullptr;

		file->seek(0);
		const auto size = file->size();

		String str;
		str.resize(size);
		// TODO: re-implement reading
		if (file->read(str.data(), size))
			return std::make_shared<TextData32>(Unicode::to_utf32(str));

		return nullptr;
	}
}