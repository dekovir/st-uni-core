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

	void StringBuilder::append(Char16 c)
	{
		const Char16 str[] = { c, 0 };
		append(str);
	}

	void StringBuilder::append(Char32 c)
	{
		const Char32 str[] = { c, 0 };
		append(str);
	}

	void StringBuilder::append(StringView text)
	{
		data.append(text);
	}

	void StringBuilder::append(StringView16 text)
	{
		data.append(Unicode::to_utf8(text));
	}

	void StringBuilder::append(StringView32 text)
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
		return builder << (value ? "true" : "false");
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

	UNICODE_STRING_BUILDER_FORMAT(const Char16)
	{
		builder.append(value);
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Char32)
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

	UNICODE_STRING_BUILDER_FORMAT(const Char16*)
	{
		if (value != nullptr)
			builder.append(value);
		return builder;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Char32*)
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