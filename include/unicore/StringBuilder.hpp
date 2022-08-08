#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class StringBuilder
	{
	public:
		String data;

		StringBuilder() noexcept = default;
		StringBuilder(const StringBuilder& other) noexcept = default;
		StringBuilder(StringBuilder&& other) noexcept;
		~StringBuilder() = default;

		void append(char c);
		void append(wchar_t c);

		void append(StringView text);
		void append(WStringView text);

		StringBuilder& operator=(const StringBuilder& other) noexcept = default;
		StringBuilder& operator=(StringBuilder&& other) noexcept;

		template<typename ... Args>
		static StringBuilder format(StringView format, const Args& ... args)
		{
			StringBuilder builder;
			internal_format(builder, format, args...);
			return builder;
		}

	protected:
		static constexpr StringView Elem = "{}";

		static void internal_format(StringBuilder& builder, StringView value)
		{
			builder.append(value);
		}

		template<typename T>
		static void internal_format(StringBuilder& builder, const T& value)
		{
			builder << value;
		}

		template<typename T, typename ... Args>
		static void internal_format(StringBuilder& builder, StringView format, const T& value, const Args& ... args)
		{
			if (const auto pos = format.find_first_of(Elem); pos != StringView::npos)
			{
				builder.append(format.substr(0, pos));
				builder << value;

				internal_format(builder, format.substr(pos + Elem.size()), args...);
			}
			else builder.append(format);
		}
	};

#define UNICODE_STRING_BUILDER_FORMAT(Type) \
	StringBuilder& operator << (StringBuilder& builder, Type value)

	extern UNICODE_STRING_BUILDER_FORMAT(int);
}