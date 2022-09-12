#pragma once
#include "unicore/Object.hpp"
#include "unicore/Memory.hpp"

namespace unicore
{
	class MemoryData : public Object
	{
	public:
		UC_NODISCARD virtual size_t size() const = 0;
		UC_NODISCARD virtual const void* data() const = 0;

		template<typename T,
			std::enable_if_t<std::is_fundamental_v<T>>* = nullptr>
		const T* as() const { return static_cast<const T*>(data()); }
	};

	class ConstMemoryData : public MemoryData
	{
	public:
		ConstMemoryData(const void* data, const size_t size)
			: _data(data), _size(size)
		{}

		UC_NODISCARD size_t size() const override { return _size; }
		UC_NODISCARD const void* data() const override { return _data; }

	protected:
		const void* _data;
		const size_t _size;
	};

	class DynamicMemoryData : public MemoryData
	{
	public:
		explicit DynamicMemoryData(size_t size)
			: _data(size)
		{}

		DynamicMemoryData(size_t size, uint8_t value)
			: _data(size, value) {}

		UC_NODISCARD size_t size() const override { return _data.size(); }
		UC_NODISCARD const void* data() const override { return _data.data(); }
		UC_NODISCARD void* data() { return _data.data(); }

		void resize(size_t new_size)
		{
			_data.resize(new_size);
		}

		void resize(size_t new_size, uint8_t value)
		{
			_data.resize(new_size, value);
		}

		void fill(uint8_t value = 0)
		{
			std::fill(_data.begin(), _data.end(), value);
		}

	protected:
		List<uint8_t> _data;
	};
}