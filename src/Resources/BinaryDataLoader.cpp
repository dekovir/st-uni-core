#include "BinaryDataLoader.hpp"

namespace unicore
{
	BinaryDataLoader::BinaryDataLoader()
		: ResourceLoaderT({ L".dat" })
	{
	}

	Shared<Resource> BinaryDataLoader::load(const Options& options)
	{
		options.file.seek(0);
		const auto size = options.file.size();

		MemoryChunk chunk(size);
		if (options.file.read(chunk.data(), size))
			return std::make_shared<BinaryData>(chunk);

		UC_LOG_ERROR(options.logger) << "Read failed";
		return nullptr;
	}
}