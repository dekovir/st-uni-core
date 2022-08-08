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

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern StringBuilder& operator << (StringBuilder& helper, const BasicStringView<TChar> value)
	{
		helper.append(value);
		return helper;
	}

	template<typename TChar,
		std::enable_if_t<std::is_same_v<TChar, char> || std::is_same_v<TChar, wchar_t>>* = nullptr>
	extern StringBuilder& operator << (StringBuilder& helper, const BasicString<TChar>& value)
	{
		helper.append(value);
		return helper;
	}

	template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& helper, T value)
	{
		const auto tmp = static_cast<int>(value);
		const auto str = std::to_wstring(tmp);
		return helper << str.c_str();
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& helper, T value)
	{
		const auto str = std::to_wstring(value);
		return helper << str.c_str();
	}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
	extern StringBuilder& operator<<(StringBuilder& helper, T value)
	{
		const auto str = std::to_wstring(value);
		return helper << str.c_str();
	}

	template<typename T>
	extern StringBuilder& operator<<(StringBuilder& helper, std::initializer_list<T> list)
	{
		int index = 0;
		for (const auto* value : list)
		{
			if (index > 0)
				helper << ',';
			helper << value;
			index++;
		}
		return helper;
	}
}