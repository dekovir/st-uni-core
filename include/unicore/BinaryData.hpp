#pragma once
#include "unicore/Resource.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class BasicBinaryData : public Resource
	{
		UC_OBJECT(BasicBinaryData, Resource)
	public:
		UC_NODISCARD virtual const void* data() const = 0;
		UC_NODISCARD virtual size_t size() const = 0;
		// TODO: Return span

		template<typename T, std::enable_if_t<std::is_trivial_v<T>>* = nullptr>
		const T* data_as() const { return static_cast<const T*>(data()); }
	};

	// BinaryData /////////////////////////////////////////////////////////////////
	class BinaryData : public BasicBinaryData
	{
		UC_OBJECT(BinaryData, BasicBinaryData)
	public:
		BinaryData(void* data, size_t size, Memory::FreeFunc free = &Memory::free);
		explicit BinaryData(MemoryChunk&& chunk);
		~BinaryData() override;

		UC_NODISCARD size_t get_system_memory_use() const override;

		UC_NODISCARD const void* data() const override { return _data; }
		UC_NODISCARD size_t size() const override { return _size; }

	protected:
		void* _data;
		size_t _size;
		Memory::FreeFunc _free;
	};

	// DynamicBinaryData //////////////////////////////////////////////////////////
	class DynamicBinaryData : public BasicBinaryData
	{
		UC_OBJECT(DynamicBinaryData, BasicBinaryData)
	public:
		explicit DynamicBinaryData(size_t size);
		DynamicBinaryData(size_t size, Byte default_value);

		UC_NODISCARD size_t get_system_memory_use() const override;

		UC_NODISCARD const void* data() const override { return _data.data(); }
		UC_NODISCARD size_t size() const override { return _data.size(); }

		UC_NODISCARD void* data() { return _data.data(); }

	protected:
		List<Byte> _data;
	};
}