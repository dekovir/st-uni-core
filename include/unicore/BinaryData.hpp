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
		BinaryData(const void* data, size_t size);
		~BinaryData() override;

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BinaryData) + _size; }

		UC_NODISCARD const void* data() const { return _data; }
		UC_NODISCARD size_t size() const { return _size; }

	protected:
		const void* _data;
		const size_t _size;
	};

	class BinaryDataLoader : public ResourceLoaderT<BinaryData>
	{
		UC_OBJECT(BinaryDataLoader, ResourceLoaderT)
	public:
		BinaryDataLoader();
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}