#include "unicore/system/Variant.hpp"
#include "unicore/system/Unicode.hpp"

namespace unicore
{
	const Variant Variant::Empty;

	Variant::Variant()
		: _data(std::nullopt) {}

	Variant::Variant(const Char* value) noexcept
		: _data(String(value)) {}

	Variant::Variant(const CharW* value) noexcept
		: _data(Unicode::to_utf32(value)) {}

	Variant::Variant(const Char16* value) noexcept
		: _data(Unicode::to_utf32(value)) {}

	Variant::Variant(const Char32* value) noexcept
		: _data(String32(value)) {}

	Variant::Variant(StringView value) noexcept
		: _data(String(value)) {}

	Variant::Variant(StringViewW value) noexcept
		: _data(Unicode::to_utf32(value)) {}

	Variant::Variant(StringView16 value) noexcept
		: _data(Unicode::to_utf32(value)) {}

	Variant::Variant(StringView32 value) noexcept
		: _data(String32(value)) {}

	bool Variant::try_get_bool(Bool& value) const
	{
		if (const auto ptr = std::get_if<Bool>(&_data))
		{
			value = *ptr;
			return true;
		}

		return false;
	}

	Bool Variant::get_bool(Bool default_value) const
	{
		Bool value;
		return try_get_bool(value) ? value : default_value;
	}

	bool Variant::try_get_int(Int& value) const
	{
		if (const auto ptr = std::get_if<Int>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Int64>(&_data))
		{
			value = static_cast<Int>(*ptr);
			return true;
		}

		if (const auto ptr = std::get_if<Float>(&_data))
		{
			value = static_cast<Int>(*ptr);
			return true;
		}

		if (const auto ptr = std::get_if<Double>(&_data))
		{
			value = static_cast<Int>(*ptr);
			return true;
		}

		return false;
	}

	Int Variant::get_int(Int default_value) const
	{
		Int value;
		return try_get_int(value) ? value : default_value;
	}

	bool Variant::try_get_int64(Int64& value) const
	{
		if (const auto ptr = std::get_if<Int>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Int64>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Float>(&_data))
		{
			value = static_cast<Int64>(*ptr);
			return true;
		}

		if (const auto ptr = std::get_if<Double>(&_data))
		{
			value = static_cast<Int64>(*ptr);
			return true;
		}

		return false;
	}

	Int64 Variant::get_int64(Int64 default_value) const
	{
		Int64 value;
		return try_get_int64(value) ? value : default_value;
	}

	bool Variant::try_get_float(Float& value) const
	{
		if (const auto ptr = std::get_if<Int>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Int64>(&_data))
		{
			value = static_cast<Float>(*ptr);
			return true;
		}

		if (const auto ptr = std::get_if<Float>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Double>(&_data))
		{
			value = static_cast<Float>(*ptr);
			return true;
		}

		return false;
	}

	Float Variant::get_float(Float default_value) const
	{
		Float value;
		return try_get_float(value) ? value : default_value;
	}

	bool Variant::try_get_double(Double& value) const
	{
		if (const auto ptr = std::get_if<Int>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Int64>(&_data))
		{
			value = static_cast<Float>(*ptr);
			return true;
		}

		if (const auto ptr = std::get_if<Float>(&_data))
		{
			value = *ptr;
			return true;
		}

		if (const auto ptr = std::get_if<Double>(&_data))
		{
			value = *ptr;
			return true;
		}

		return false;
	}

	Double Variant::get_double(Double default_value) const
	{
		Double value;
		return try_get_double(value) ? value : default_value;
	}

	bool Variant::try_get_string(String& value) const
	{
		if (const auto str = std::get_if<String>(&_data))
		{
			value = *str;
			return true;
		}

		if (const auto str = std::get_if<String32>(&_data))
		{
			value = Unicode::to_utf8(*str);
			return true;
		}

		return false;
	}

	String Variant::get_string(StringView default_value) const
	{
		String str;
		return try_get_string(str) ? str : String(default_value);
	}

	bool Variant::try_get_string32(String32& value) const
	{
		if (const auto str = std::get_if<String32>(&_data))
		{
			value = *str;
			return true;
		}

		if (const auto str = std::get_if<String>(&_data))
		{
			value = Unicode::to_utf32(*str);
			return true;
		}

		return false;
	}

	String32 Variant::get_string32(StringView32 default_value) const
	{
		String32 str;
		return try_get_string32(str) ? str : String32(default_value);
	}
}