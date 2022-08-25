#pragma once
#include <any>
#include "unicore/Object.hpp"

namespace unicore
{
	enum class ResourceCachePolicy : uint8_t
	{
		CanCache,
		NoCache,
	};

	class Resource : public virtual Object
	{
		UC_OBJECT(Resource, Object)
	public:
		UC_NODISCARD virtual size_t get_system_memory_use() const = 0;

		UC_NODISCARD virtual size_t get_used_resources(Set<Shared<Resource>>& resources) { return 0; }
		UC_NODISCARD virtual ResourceCachePolicy cache_policy() const { return ResourceCachePolicy::CanCache; }
	};

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceList : public Resource
	{
		UC_OBJECT(ResourceList, Resource)
	public:
		using DataType = List<Shared<T>>;

		explicit ResourceList(DataType data)
			: _data(std::move(data))
		{}

		auto operator[](size_t index) const { return _data[index]; }

		UC_NODISCARD const DataType& data() const { return _data; }
		UC_NODISCARD size_t size() const { return _data.size(); }

		
		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(ResourceList); }
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override
		{
			resources.insert(_data.begin(), _data.end());
			return _data.size();
		}

	protected:
		const DataType _data;
	};

	template<typename T,
		std::enable_if_t<std::is_base_of_v<Resource, T>>* = nullptr>
	class ResourceAtlas : public Resource
	{
		UC_OBJECT(ResourceAtlas, Resource)
	public:
		using ResourceList = List<Shared<T>>;
		using NamesDictionary = Dictionary<String, uint16_t>;

		ResourceAtlas(ResourceList list, NamesDictionary names)
			:_list(std::move(list)), _names(std::move(names))
		{}

		UC_NODISCARD const ResourceList& list() const { return _list; }
		UC_NODISCARD const NamesDictionary& names() const { return _names; }

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(ResourceAtlas); }
		UC_NODISCARD size_t get_used_resources(Set<Shared<Resource>>& resources) override
		{
			resources.insert(_list.begin(), _list.end());
			return _list.size();
		}

		Shared<T> find(StringView name) const
		{
			const auto it = _names.find(name.data());
			return it != _names.end() ? _list[it->second] : nullptr;
		}

	protected:
		const ResourceList _list;
		const NamesDictionary _names;
	};
}