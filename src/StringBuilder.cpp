#include "unicore/StringBuilder.hpp"
#include "unicore/Strings.hpp"

namespace unicore
{
	StringBuilder::StringBuilder(StringBuilder&& other) noexcept
		: data(std::move(other.data))
	{
	}

	void StringBuilder::append(char c)
	{
		const char str[] = { c, 0 };
		append(str);
	}

	void StringBuilder::append(wchar_t c)
	{
		const wchar_t str[] = { c, 0 };
		append(str);
	}

	void StringBuilder::append(StringView text)
	{
		data.append(text);
	}

	void StringBuilder::append(WStringView text)
	{
		data.append(Strings::to_utf8(text));
	}

	StringBuilder& StringBuilder::operator=(StringBuilder&& other) noexcept
	{
		data = std::move(other.data);
		return *this;
	}

	UNICODE_STRING_BUILDER_FORMAT(const StringBuilder&)
	{
		return builder << value.data;
	}

	UNICODE_STRING_BUILDER_FORMAT(bool)
	{
		return builder << (value ? L"true" : L"false");
	}

#if defined (_HAS_EXCEPTIONS)
	UNICODE_STRING_BUILDER_FORMAT(std::exception&)
	{
		return builder << value.what();
	}
#endif

	UNICODE_STRING_BUILDER_FORMAT(const char)
	{
		builder.append(value);
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const wchar_t)
	{
		builder.append(value);
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const char*)
	{
		if (value != nullptr)
			builder.append(value);
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const wchar_t*)
	{
		if (value != nullptr)
			builder.append(value);
		return builder;
	}
}