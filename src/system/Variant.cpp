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
	Bool Variant::is_bool() const
	{
		return std::holds_alternative<Bool>(_data);
	}

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
	Bool Variant::is_int() const
	{
		return std::holds_alternative<Int>(_data);
	}

	Bool Variant::is_int64() const
	{
		return std::holds_alternative<Int64>(_data);
	}

	Bool Variant::is_integral() const
	{
		return is_int() || is_int64();
	}

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
	Bool Variant::is_float() const
	{
		return std::holds_alternative<Float>(_data);
	}

	Bool Variant::is_double() const
	{
		return std::holds_alternative<Double>(_data);
	}

	Bool Variant::is_floating_point() const
	{
		return is_float() || is_double();
	}

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
	Bool Variant::is_string() const
	{
		return std::holds_alternative<String>(_data);
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
	Bool Variant::is_string32() const
	{
		return std::holds_alternative<String32>(_data);
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

	Bool Variant::is_any_string() const
	{
		return is_string() || is_string32();
	}

	// VECTOR2 /////////////////////////////////////////////////////////////////
	Bool Variant::is_vec2i() const
	{
		return std::holds_alternative<Vector2i>(_data);
	}

	Bool Variant::is_vec2f() const
	{
		return std::holds_alternative<Vector2f>(_data);
	}

	Bool Variant::is_vec2() const
	{
		return is_vec2i() || is_vec2f();
	}

	bool Variant::try_get_vec2i(Vector2i& value) const
	{
		return try_get_vec2(value);
	}

	Vector2i Variant::get_vec2i(const Vector2i& default_value) const
	{
		return get_vec2(default_value);
	}

	bool Variant::try_get_vec2f(Vector2f& value) const
	{
		return try_get_vec2(value);
	}

	Vector2f Variant::get_vec2f(const Vector2f& default_value) const
	{
		return get_vec2(default_value);
	}

	// VECTOR3 /////////////////////////////////////////////////////////////////
	Bool Variant::is_vec3i() const
	{
		return std::holds_alternative<Vector3i>(_data);
	}

	Bool Variant::is_vec3f() const
	{
		return std::holds_alternative<Vector3f>(_data);
	}

	Bool Variant::is_vec3() const
	{
		return is_vec3i() || is_vec3f();
	}

	bool Variant::try_get_vec3i(Vector3i& value) const
	{
		return try_get_vec3(value);
	}

	Vector3i Variant::get_vec3i(const Vector3i& default_value) const
	{
		return get_vec3(default_value);
	}

	bool Variant::try_get_vec3f(Vector3f& value) const
	{
		return try_get_vec3(value);
	}

	Vector3f Variant::get_vec3f(const Vector3f& default_value) const
	{
		return get_vec3(default_value);
	}

	// RANGE ///////////////////////////////////////////////////////////////////
	Bool Variant::is_rangei() const
	{
		return std::holds_alternative<Rangei>(_data);
	}

	Bool Variant::is_rangef() const
	{
		return std::holds_alternative<Rangef>(_data);
	}

	Bool Variant::is_range() const
	{
		return is_rangei() || is_rangef();
	}

	bool Variant::try_get_rangei(Rangei& value) const
	{
		return try_get_range(value);
	}

	Rangei Variant::get_rangei(const Rangei& default_value) const
	{
		return get_range(default_value);
	}

	bool Variant::try_get_rangef(Rangef& value) const
	{
		return try_get_range(value);
	}

	Rangef Variant::get_rangef(const Rangef& default_value) const
	{
		return get_range(default_value);
	}

	// RECT ////////////////////////////////////////////////////////////////////
	Bool Variant::is_recti() const
	{
		return std::holds_alternative<Recti>(_data);
	}

	Bool Variant::is_rectf() const
	{
		return std::holds_alternative<Rectf>(_data);
	}

	Bool Variant::is_rect() const
	{
		return is_recti() || is_rectf();
	}

	bool Variant::try_get_recti(Recti& value) const
	{
		return try_get_rect(value);
	}

	Recti Variant::get_recti(const Recti& default_value) const
	{
		return get_rect(default_value);
	}

	bool Variant::try_get_rectf(Rectf& value) const
	{
		return try_get_rect(value);
	}

	Rectf Variant::get_rectf(const Rectf& default_value) const
	{
		return get_rect(default_value);
	}

	// COLOR3 //////////////////////////////////////////////////////////////////
	Bool Variant::is_color3b() const
	{
		return std::holds_alternative<Color3b>(_data);
	}

	Bool Variant::is_color3f() const
	{
		return std::holds_alternative<Color3f>(_data);
	}

	Bool Variant::is_color3() const
	{
		return is_color3b() || is_color3f();
	}

	bool Variant::try_get_color3b(Color3b& value) const
	{
		return try_get_color3(value);
	}

	Color3b Variant::get_color3b(const Color3b& default_value) const
	{
		return get_color3(default_value);
	}

	bool Variant::try_get_color3f(Color3f& value) const
	{
		return try_get_color3(value);
	}

	Color3f Variant::get_color3f(const Color3f& default_value) const
	{
		return get_color3(default_value);
	}

	// COLOR4 //////////////////////////////////////////////////////////////////
	Bool Variant::is_color4b() const
	{
		return std::holds_alternative<Color4b>(_data);
	}

	Bool Variant::is_color4f() const
	{
		return std::holds_alternative<Color4f>(_data);
	}

	Bool Variant::is_color4() const
	{
		return is_color4b() || is_color4f();
	}

	bool Variant::try_get_color4b(Color4b& value) const
	{
		return try_get_color4(value);
	}

	Color4b Variant::get_color4b(const Color4b& default_value) const
	{
		return get_color4(default_value);
	}

	bool Variant::try_get_color4f(Color4f& value) const
	{
		return try_get_color4(value);
	}

	Color4f Variant::get_color4f(const Color4f& default_value) const
	{
		return get_color4(default_value);
	}

	// OBJECT ////////////////////////////////////////////////////////////////////
	Bool Variant::is_object() const
	{
		return std::holds_alternative<Shared<Object>>(_data);
	}

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