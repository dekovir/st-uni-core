#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class BasicBinaryData : public Resource
	{
		UC_OBJECT(BasicBinaryData, Resource)
	public:
		UC_NODISCARD virtual const Byte* data() const = 0;
		UC_NODISCARD virtual size_t size() const = 0;
		// TODO: Return span
	};

	class BinaryData : public BasicBinaryData
	{
		UC_OBJECT(BinaryData, BasicBinaryData)
	public:
		explicit BinaryData(MemoryChunk chunk);

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BinaryData) + _chunk.size(); }

		UC_NODISCARD const Byte* data() const override { return static_cast<const Byte*>(_chunk.data()); }
		UC_NODISCARD size_t size() const override { return _chunk.size(); }

	protected:
		MemoryChunk _chunk;
	};

	class DynamicBinaryData : public BasicBinaryData
	{
		UC_OBJECT(DynamicBinaryData, BasicBinaryData)
	public:
		explicit DynamicBinaryData(size_t size) : _data(size) {}
		DynamicBinaryData(size_t size, Byte default_value) : _data(size, default_value) {}

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(DynamicBinaryData) + _data.size(); }

		UC_NODISCARD const Byte* data() const override { return _data.data(); }
		UC_NODISCARD size_t size() const override { return _data.size(); }

		UC_NODISCARD Byte* data() { return _data.data(); }

	protected:
		List<Byte> _data;
	};
}