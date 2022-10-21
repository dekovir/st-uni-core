#include "unicore/system/Variant.hpp"
#include "unicore/system/Unicode.hpp"

namespace unicore
{
	const Variant Variant::Empty;

	Variant::Variant() = default;

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

	// BOOL //////////////////////////////////////////////////////////////////////
	bool Variant::try_get_bool(Bool& value) const
	{
		if (const auto ptr = std::get_if<Bool>(&_data))
		{
			value = *ptr;
			return true;
		}

		// TODO: Convert from integral?
		// TODO: Convert from String? (true/false)

		return false;
	}

	Bool Variant::get_bool(Bool default_value) const
	{
		Bool value;
		return try_get_bool(value) ? value : default_value;
	}

	// INT ///////////////////////////////////////////////////////////////////////
	bool Variant::try_get_int(Int& value) const
	{
		if (try_get_integral(value))
			return true;

		Float f;
		if (try_get_floating_point(f))
		{
			value = static_cast<Int>(f);
			return true;
		}

		// TODO: Convert from bool

		return false;
	}

	Int Variant::get_int(Int default_value) const
	{
		Int value;
		return try_get_int(value) ? value : default_value;
	}

	// INT64 /////////////////////////////////////////////////////////////////////
	bool Variant::try_get_int64(Int64& value) const
	{
		if (try_get_integral(value))
			return true;

		Double d;
		if (try_get_floating_point(d))
		{
			value = static_cast<Int64>(d);
			return true;
		}

		// TODO: Convert from bool

		return false;
	}

	Int64 Variant::get_int64(Int64 default_value) const
	{
		Int64 value;
		return try_get_int64(value) ? value : default_value;
	}

	// FLOAT /////////////////////////////////////////////////////////////////////
	bool Variant::try_get_float(Float& value) const
	{
		if (try_get_floating_point(value))
			return true;

		Int64 i;
		if (try_get_integral(i))
		{
			value = static_cast<Float>(i);
			return true;
		}

		return false;
	}

	Float Variant::get_float(Float default_value) const
	{
		Float value;
		return try_get_float(value) ? value : default_value;
	}

	// DOUBLE ////////////////////////////////////////////////////////////////////
	bool Variant::try_get_double(Double& value) const
	{
		if (try_get_floating_point(value))
			return true;

		Int64 i;
		if (try_get_integral(i))
		{
			value = static_cast<Double>(i);
			return true;
		}

		return false;
	}

	Double Variant::get_double(Double default_value) const
	{
		Double value;
		return try_get_double(value) ? value : default_value;
	}

	// STRING ////////////////////////////////////////////////////////////////////
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

		// TODO: Convert from boolean?
		// TODO: Convert from integral?
		// TODO: Convert from floating point?

		return false;
	}

	String Variant::get_string(StringView default_value) const
	{
		String str;
		return try_get_string(str) ? str : String(default_value);
	}

	// STRING32 //////////////////////////////////////////////////////////////////
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

		// TODO: Convert from boolean?
		// TODO: Convert from integral?
		// TODO: Convert from floating point?

		return false;
	}

	String32 Variant::get_string32(StringView32 default_value) const
	{
		String32 str;
		return try_get_string32(str) ? str : String32(default_value);
	}

	UNICODE_STRING_BUILDER_FORMAT(const Variant&)
	{
		builder << "V(";

		Bool b;
		Int64 i64;
		Double d;
		String32 s32;

		if (value.try_get_bool(b))
			builder << b;
		else if (value.try_get_int64(i64))
			builder << i64;
		else if (value.try_get_double(d))
			builder << d;
		else if (value.try_get_string32(s32))
			builder << s32;
		else builder << "<?>";

		return builder << ")";
	}
}