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

	UNICODE_STRING_BUILDER_FORMAT(int)
	{
		const auto str = std::to_string(value);
		builder.append(str);
		return builder;
	}
}