#pragma once
#include "unicore/Resource.hpp"
#include "unicore/ResourceLoader.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class BinaryData : public Resource
	{
		UC_OBJECT(BinaryData, Resource)
	public:
		explicit BinaryData(size_t size);
		explicit BinaryData(const Shared<MemoryChunk>& chunk);

		explicit BinaryData(const BinaryData& other);
		BinaryData(BinaryData&& other) noexcept;

		~BinaryData() = default;

		BinaryData& operator=(const BinaryData& other);
		BinaryData& operator=(BinaryData&& other) noexcept;

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BinaryData) + _chunk->size(); }

		UC_NODISCARD void* data() { return _chunk->data(); }
		UC_NODISCARD const void* data() const { return _chunk->data(); }

		UC_NODISCARD size_t size() const { return _chunk->size(); }

	protected:
		Shared<MemoryChunk> _chunk;
	};

	class BinaryDataLoader : public ResourceLoaderT<BinaryData>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderT)
	public:
		BinaryDataLoader();
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}