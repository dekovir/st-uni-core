#pragma once
#include <utility>

#include "unicore/Resource.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	template<typename Char>
	class BasicStringData : public Resource
	{
		UC_OBJECT(BasicStringData, Resource)
	public:
		explicit BasicStringData(BasicStringView<Char> data)
			: _data(data) {}

		explicit BasicStringData(BasicString<Char> data)
			: _data(std::move(data)) {}

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BasicStringData<Char>) + _data.size() * sizeof(Char); }

		UC_NODISCARD const BasicString<Char>& data() const { return _data; }

	protected:
		const BasicString<Char> _data;
	};

	class StringData : public BasicStringData<char>
	{
		UC_OBJECT(StringData, BasicStringData)
	public:
		explicit StringData(StringView data)
			: BasicStringData<char>(data)
		{
		}

		explicit StringData(String data)
			: BasicStringData<char>(std::move(data))
		{
		}
	};

	class StringDataLoader : public ResourceLoaderT<StringData>
	{
		UC_OBJECT(StringDataLoader, ResourceLoaderT)
	public:
		StringDataLoader();
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	class WStringData : public BasicStringData<wchar_t>
	{
	public:
		explicit WStringData(WStringView data)
			: BasicStringData<wchar_t>(data)
		{
		}

		explicit WStringData(WString data)
			: BasicStringData<wchar_t>(std::move(data))
		{
		}
	};
}