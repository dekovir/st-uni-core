#pragma once
#include <utility>

#include "unicore/Resource.hpp"
#include "unicore/ResourceLoader.hpp"

namespace unicore
{
	template<typename Char>
	class BasicTextData : public Resource
	{
		UC_OBJECT(BasicTextData, Resource)
	public:
		explicit BasicTextData(BasicStringView<Char> data)
			: _data(data) {}

		explicit BasicTextData(BasicString<Char> data)
			: _data(std::move(data)) {}

		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(BasicTextData<Char>) + _data.size() * sizeof(Char); }

		UC_NODISCARD const BasicString<Char>& data() const { return _data; }

	protected:
		const BasicString<Char> _data;
	};

	class TextData : public BasicTextData<char>
	{
		UC_OBJECT(TextData, BasicTextData)
	public:
		explicit TextData(StringView data)
			: BasicTextData<char>(data)
		{
		}

		explicit TextData(String data)
			: BasicTextData<char>(std::move(data))
		{
		}
	};

	class WTextData : public BasicTextData<wchar_t>
	{
	public:
		explicit WTextData(WStringView data)
			: BasicTextData<wchar_t>(data)
		{
		}

		explicit WTextData(WString data)
			: BasicTextData<wchar_t>(std::move(data))
		{
		}
	};

	class TextDataLoader : public ResourceLoaderT<TextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderT)
	public:
		TextDataLoader();
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};

	class WTextDataLoader : public ResourceLoaderT<WTextData>
	{
		UC_OBJECT(TextDataLoader, ResourceLoaderT)
	public:
		WTextDataLoader();
		UC_NODISCARD Shared<Resource> load(const ResourceLoaderContext& context) override;
	};
}