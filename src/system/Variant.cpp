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

		uint8_t i;
		if (try_get_integral(i))
		{
			switch (i)
			{
			case 0:
				value = false;
				return true;

			case 1:
				value = true;
				return true;
			}
		}

		String str;
		if (try_get_string(str))
		{
			if (StringHelper::equals(str, "true", true))
			{
				value = true;
				return true;
			}

			if (StringHelper::equals(str, "false", true))
			{
				value = true;
				return true;
			}
		}

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

		int64_t i64;
		if (try_get_integral(i64))
		{
			value = std::to_string(i64);
			return true;
		}

		Double d;
		if (try_get_floating_point(d))
		{
			value = std::to_string(d);
			return true;
		}

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

		int64_t i64;
		if (try_get_integral(i64))
		{
			value = Unicode::to_utf32(std::to_string(i64));
			return true;
		}

		Double d;
		if (try_get_floating_point(d))
		{
			value = Unicode::to_utf32(std::to_string(d));
			return true;
		}

		return false;
	}

	String32 Variant::get_string32(StringView32 default_value) const
	{
		String32 str;
		return try_get_string32(str) ? str : String32(default_value);
	}

	// OBJECT ////////////////////////////////////////////////////////////////////
	bool Variant::try_get_object(Shared<Object>& value) const
	{
		if (const auto ptr = std::get_if<Shared<Object>>(&_data))
		{
			value = *ptr;
			return true;
		}

		return false;
	}

	Shared<Object> Variant::get_object(const Shared<Object>& default_value) const
	{
		Shared<Object> value;
		return try_get_object(value) ? value : default_value;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Variant&)
	{
		builder << "V(";
		builder << value.data().index();
		builder << ":";

		if (Bool tmp; value.try_get_bool(tmp))
			builder << tmp;
		else if (Int64 tmp; value.try_get_integral(tmp))
			builder << tmp;
		else if (Double tmp; value.try_get_floating_point(tmp))
			builder << tmp;

		else if (String32 tmp; value.try_get_string32(tmp))
			builder << tmp;

		else if (Vector2i tmp; value.try_get_vec2i(tmp))
			builder << "[" << tmp << "]";
		else if (Vector2f tmp; value.try_get_vec2f(tmp))
			builder << "[" << tmp << "]";
		else if (Vector3i tmp; value.try_get_vec3i(tmp))
			builder << "[" << tmp << "]";
		else if (Vector3f tmp; value.try_get_vec3f(tmp))
			builder << "[" << tmp << "]";

		else if (Rangei tmp; value.try_get_rangei(tmp))
			builder << "[" << tmp << "]";
		else if (Rangef tmp; value.try_get_rangef(tmp))
			builder << "[" << tmp << "]";

		else if (Recti tmp; value.try_get_recti(tmp))
			builder << "[" << tmp << "]";
		else if (Rectf tmp; value.try_get_rectf(tmp))
			builder << "[" << tmp << "]";

		else if (Color3b tmp; value.try_get_color3b(tmp))
			builder << "[" << tmp << "]";
		else if (Color4b tmp; value.try_get_color4b(tmp))
			builder << "[" << tmp << "]";
		else if (Shared<Object> tmp; value.try_get_object(tmp))
			tmp ? builder << tmp->type() : builder << "null";
		else builder << "?";

		return builder << ")";
	}
}