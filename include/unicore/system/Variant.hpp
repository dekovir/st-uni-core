#pragma once
#include "unicore/math/Rect.hpp"
#include "unicore/renderer/Color3.hpp"
#include "unicore/renderer/Color4.hpp"

namespace unicore
{
	class Variant
	{
	public:
		using DataType = StdVariant<
			std::nullopt_t,
			Bool, Int, Int64,
			Float, Double,
			String, String32,
			Vector2i, Vector2f, Recti, Rectf,
			Color3b, Color4b, Color3f, Color4f
		>;

		Variant();

		Variant(const Char* value) noexcept;
		Variant(const CharW* value) noexcept;
		Variant(const Char16* value) noexcept;
		Variant(const Char32* value) noexcept;

		Variant(StringView value) noexcept;
		Variant(StringViewW value) noexcept;
		Variant(StringView16 value) noexcept;
		Variant(StringView32 value) noexcept;

		template<typename T,
			std::enable_if_t<std::is_constructible_v<DataType, T>>* = nullptr>
		Variant(T value) noexcept : _data(value) {}

		UC_NODISCARD const DataType& data() const { return _data; }
		UC_NODISCARD Bool empty() const { return std::holds_alternative<std::nullopt_t>(_data); }

		UC_NODISCARD bool try_get_bool(Bool& value) const;
		UC_NODISCARD Bool get_bool(Bool default_value = false) const;

		UC_NODISCARD bool try_get_int(Int& value) const;
		UC_NODISCARD Int get_int(Int default_value = 0) const;

		UC_NODISCARD bool try_get_int64(Int64& value) const;
		UC_NODISCARD Int64 get_int64(Int64 default_value = 0) const;

		UC_NODISCARD bool try_get_float(Float& value) const;
		UC_NODISCARD Float get_float(Float default_value = 0) const;

		UC_NODISCARD bool try_get_double(Double& value) const;
		UC_NODISCARD Double get_double(Double default_value = 0) const;

		UC_NODISCARD bool try_get_string(String& value) const;
		UC_NODISCARD String get_string(StringView default_value = "") const;

		UC_NODISCARD bool try_get_string32(String32& value) const;
		UC_NODISCARD String32 get_string32(StringView32 default_value = U"") const;

		static const Variant Empty;

	protected:
		DataType _data;
	};
}