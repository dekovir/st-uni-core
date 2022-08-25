#include "unicore/StringBuilder.hpp"
#include "unicore/Unicode.hpp"

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
		data.append(Unicode::to_utf8(text));
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

	UNICODE_STRING_BUILDER_FORMAT(float)
	{
		// TODO: Implement precision
		return builder << StringHelper::print_format("%.2f", value);
	}

	UNICODE_STRING_BUILDER_FORMAT(double)
	{
		// TODO: Implement precision
		return builder << StringHelper::print_format("%.2lf", value);
	}

	UNICODE_STRING_BUILDER_FORMAT(const std::type_info&)
	{
		return builder << "<" << value.name() << ">";
	}

	UNICODE_STRING_BUILDER_FORMAT(const std::type_index&)
	{
		return builder << "<" << value.name() << ">";
	}
}