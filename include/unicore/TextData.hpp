#pragma once
#include "unicore/Resource.hpp"

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

		class TextData16 : public BasicTextData<Char16>
	{
		UC_OBJECT(TextData16, BasicTextData)
	public:
		explicit TextData16(StringView16 data)
			: BasicTextData(data)
		{
		}

		explicit TextData16(String16 data)
			: BasicTextData(std::move(data))
		{
		}
	};

	class TextData32 : public BasicTextData<Char32>
	{
		UC_OBJECT(TextData32, BasicTextData)
	public:
		explicit TextData32(StringView32 data)
			: BasicTextData(data)
		{
		}

		explicit TextData32(String32 data)
			: BasicTextData(std::move(data))
		{
		}
	};
}