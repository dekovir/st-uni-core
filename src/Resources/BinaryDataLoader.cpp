#include "BinaryDataLoader.hpp"

namespace unicore
{
	BinaryDataLoader::BinaryDataLoader()
		: ResourceLoaderT({ L".dat" })
	{
	}

	Shared<Resource> BinaryDataLoader::load(const ResourceLoaderContext& context)
	{
		context.stream.seek(0);
		const auto size = context.stream.size();

		MemoryChunk chunk(size);
		if (context.stream.read(chunk.data(), size))
			return std::make_shared<BinaryData>(chunk);

		UC_LOG_ERROR(context.logger) << "Read failed";
		return nullptr;
	}
}