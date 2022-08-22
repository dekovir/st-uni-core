#pragma once
#include "unicore/StringHelper.hpp"

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
		static String format(StringView format, const Args& ... args)
		{
			StringBuilder builder;
			internal_format(builder, format, args...);
			return builder.data;
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

	extern UNICODE_STRING_BUILDER_FORMAT(const StringBuilder&);

	extern UNICODE_STRING_BUILDER_FORMAT(bool);

#if defined (_HAS_EXCEPTIONS)
	extern UNICODE_STRING_BUILDER_FORMAT(const std::exception&);
#endif

	extern UNICODE_STRING_BUILDER_FORMAT(char);
	extern UNICODE_STRING_BUILDER_FORMAT(wchar_t);

	extern UNICODE_STRING_BUILDER_FORMAT(const char*);
	extern UNICODE_STRING_BUILDER_FORMAT(const wchar_t*);

	extern UNICODE_STRING_BUILDER_FORMAT(float);
	extern UNICODE_STRING_BUILDER_FORMAT(double);

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern StringBuilder& operator << (StringBuilder& builder, const BasicStringView<TChar> value)
	{
		builder.append(value);
		return builder;
	}

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern StringBuilder& operator << (StringBuilder& builder, const BasicString<TChar>& value)
	{
		builder.append(value);
		return builder;
	}

	template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& builder, T value)
	{
		const auto tmp = static_cast<int>(value);
		const auto str = std::to_string(tmp);
		return builder << str.c_str();
	}

	template<typename T, std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& builder, T value)
	{
		return builder << "0x" << StringHelper::to_hex(reinterpret_cast<intptr_t>(value));
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& builder, T value)
	{
		const auto str = std::to_string(value);
		return builder << str.c_str();
	}

	template<typename T>
	extern StringBuilder& operator<<(StringBuilder& builder, std::initializer_list<T> list)
	{
		int index = 0;
		for (const auto* value : list)
		{
			if (index > 0)
				builder << ',';
			builder << value;
			index++;
		}
		return builder;
	}
}