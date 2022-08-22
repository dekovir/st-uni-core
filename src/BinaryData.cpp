#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	BinaryData::BinaryData(const void* data, size_t size)
		: _data(data), _size(size)
	{
	}

	BinaryData::~BinaryData()
	{
		Memory::free(_data);
	}

	BinaryDataLoader::BinaryDataLoader()
		: ResourceLoaderT({ L".dat" })
	{
	}

	Shared<Resource> BinaryDataLoader::load(const ResourceLoaderContext& context)
	{
		context.stream.seek(0);
		const auto size = context.stream.size();

		auto buffer = UC_ALLOC(size);
		if (context.stream.read(buffer, size))
			return std::make_shared<BinaryData>(buffer, size);

		UC_FREE(buffer);
		UC_LOG_ERROR(context.logger) << "Read failed";
		return nullptr;
	}
}