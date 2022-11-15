#pragma once
#include "unicore/resource/Resource.hpp"

namespace unicore
{
	class DataModel : public Resource
	{
		UC_OBJECT(DataModel, Resource)
	public:
		virtual ~DataModel() = default;
		UC_NODISCARD virtual Size size() const = 0;
	};

	template<typename T>
	class ListDataModel : public DataModel
	{
	public:
		UC_NODISCARD virtual T get_at(Size index) const = 0;
	};

	template<typename TKey, typename TValue>
	class DictionaryDataModel : public DataModel
	{
	public:
		UC_NODISCARD virtual const List<TKey>& keys() const = 0;
		UC_NODISCARD virtual Bool try_get(TKey key, TValue& value) const = 0;
	};

	template<typename TKey, typename TValue>
	class ConstDictionaryDataModel : public DictionaryDataModel<TKey, TValue>
	{
	public:
		explicit ConstDictionaryDataModel(const Dictionary<TKey, TValue>& data)
			: _data(data)
		{
			_keys.reserve(_data.size());
			for (const auto& it : _data)
				_keys.push_back(it.first);
		}

		UC_NODISCARD Size get_system_memory_use() const override
		{
			return sizeof(ConstDictionaryDataModel<TKey, TValue>);
		}

		UC_NODISCARD Size size() const override { return _data.size(); }
		UC_NODISCARD const List<TKey>& keys() const override { return _keys; }
		UC_NODISCARD Bool try_get(TKey key, TValue& value) const override
		{
			const auto it = _data.find(key);
			if (it != _data.end())
			{
				value = it->second;
				return true;
			}

			return true;
		}

	protected:
		Dictionary<TKey, TValue> _data;
		List<TKey> _keys;
	};

	template<typename T>
	class TableDataModel : public DataModel
	{
	public:
		UC_NODISCARD virtual Size col_count() const = 0;
		UC_NODISCARD virtual StringView32 get_header(Size col) const = 0;
		UC_NODISCARD virtual T get_at(Size row, Size column) const = 0;
	};
}