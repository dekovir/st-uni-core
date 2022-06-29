#include "unicore/Data.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	Shared<Resource> BinaryDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto size = context.stream.size();
		context.stream.seek(0);
		auto buffer = make_shared<MemoryChunk>(size);
		if (context.stream.read(buffer->data(), size))
			return std::make_shared<BinaryData>(buffer);

		UC_LOG_ERROR(context.logger) << "Read failed";
		return nullptr;
	}

	Shared<Resource> TextDataLoader::load(const ResourceLoaderContext& context)
	{
		const auto size = context.stream.size();
		context.stream.seek(0);
		String str;
		str.resize(size);
		if (context.stream.read(str.data(), size))
			return std::make_shared<TextData>(str);

		return nullptr;
	}

	
}