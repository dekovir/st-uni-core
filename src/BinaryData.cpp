#include <utility>

#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	BinaryData::BinaryData(MemoryChunk chunk)
		: _chunk(std::move(chunk))
	{
	}

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