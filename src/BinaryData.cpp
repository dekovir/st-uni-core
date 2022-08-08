#include "unicore/BinaryData.hpp"
#include "unicore/Memory.hpp"
#include "unicore/Stream.hpp"
#include "unicore/ResourceCache.hpp"

namespace unicore
{
	BinaryData::BinaryData(size_t size)
		: _chunk(std::make_shared<MemoryChunk>(size))
	{
	}

	BinaryData::BinaryData(const Shared<MemoryChunk>& chunk)
		: _chunk(chunk)
	{}

	BinaryData::BinaryData(const BinaryData& other)
		: _chunk(other._chunk)
	{
	}

	BinaryData::BinaryData(BinaryData&& other) noexcept
		: _chunk(std::move(other._chunk))
	{
	}

	BinaryData& BinaryData::operator=(const BinaryData& other)
	{
		if (_chunk && other._chunk)
			*_chunk = *other._chunk;
		else if (other._chunk)
		{
			_chunk = std::make_shared<MemoryChunk>(*other._chunk);
		}
		else
		{
			_chunk = nullptr;
		}
		return *this;
	}

	BinaryData& BinaryData::operator=(BinaryData&& other) noexcept
	{
		_chunk = std::exchange(other._chunk, nullptr);
		return *this;
	}

	BinaryDataLoader::BinaryDataLoader()
		: ResourceLoaderT({ L".dat" })
	{
	}

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
}