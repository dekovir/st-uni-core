#pragma once
#include "unicore/resource/Resource.hpp"
#include "unicore/math/Vector2.hpp"

namespace unicore
{
	class DataModel : public Resource
	{
		UC_OBJECT(DataModel, Resource)
	public:
	};

	namespace sfinae
	{
		template<Bool Value, class A = void, class B = void>
		struct enable_if_else
		{
			using type = A;
		};

		template<class A, class B>
		struct enable_if_else<false, A, B>
		{
			using type = B;
		};

		template<Bool Value, class A = void, class B = void>
		using enable_if_else_t = typename enable_if_else<Value, A, B>::type;

		template<class T>
		inline constexpr bool is_small_v = sizeof(T) <= sizeof(intptr_t);

		template<typename T>
		using ConstRefType = enable_if_else_t<is_small_v<T>, T, const T&>;
	}

	template<typename TData, typename TIndex>
	class DataModelTyped : public DataModel
	{
	public:
		using DataType = TData;
		using IndexType = TIndex;
		using IndexArg = sfinae::ConstRefType<TIndex>;

		UC_NODISCARD virtual IndexArg size() const = 0;
		UC_NODISCARD virtual DataType get_at(IndexArg index) const = 0;
	};

	template<typename TData>
	using DataModel1 = DataModelTyped<TData, Size>;

	template<typename TData>
	using DataModel2 = DataModelTyped<TData, Vector2<Size>>;

	template<typename TKey, typename TValue>
	class DictionaryDataModel : public DataModel1<TKey>
	{
		UC_OBJECT(DictionaryDataModel, DataModel1<TKey>)
	public:
		using ValueType = TValue;
		using ValueArg = sfinae::ConstRefType<ValueType>;

		UC_NODISCARD virtual const List<TKey>& keys() const = 0;
		UC_NODISCARD typename DataModel1<TKey>::DataType get_at(typename DataModel1<TKey>::IndexArg index) const override
		{
			return keys()[index];
		}

		UC_NODISCARD virtual Bool try_get_value(TKey key, ValueType& value) const = 0;
		UC_NODISCARD virtual TValue get_value(TKey key, ValueArg default_value = {}) const
		{
			TValue value = {};
			return try_get_value(key, value) ? value : default_value;
		}
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
		UC_NODISCARD Bool try_get_value(TKey key,
			typename DictionaryDataModel<TKey, TValue>::ValueType& value) const override
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
	class TableDataModel : public DataModel2<T>
	{
		UC_OBJECT(TableDataModel, DataModel)
	public:
		UC_NODISCARD virtual StringView32 get_header(Size col) const = 0;
	};
}