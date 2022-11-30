#pragma once
#include "unicore/math/Rect.hpp"
#include "unicore/math/Vector3.hpp"
#include "unicore/renderer/Color3.hpp"
#include "unicore/renderer/Color4.hpp"
#include "unicore/system/Object.hpp"

namespace unicore
{
	class Variant
	{
	public:
		using DataType = StdVariant<
			Bool, Int, Int64,
			Float, Double,
			String, String32,
			Vector2i, Vector2f, Vector3i, Vector3f,
			Rangei, Rangef, Recti, Rectf,
			Color3b, Color3f, Color4b, Color4f,
			Shared<Object>
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

		template<typename T, std::enable_if_t<std::is_enum_v<T>>* = nullptr>
		Variant(T enum_value) noexcept : _data(static_cast<std::underlying_type_t<T>>(enum_value)) {}

		template<typename T,
			std::enable_if_t<std::is_constructible_v<DataType, T>>* = nullptr>
		Variant(T value) noexcept : _data(value) {}

		template<typename T,
			std::enable_if_t<!std::is_constructible_v<DataType, T>&& std::is_integral_v<T>>* = nullptr>
		Variant(T value) noexcept : _data(static_cast<Int64>(value)) {}

		template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
		Variant(const Shared<T>& object) noexcept : _data(object) {}

		UC_NODISCARD const DataType& data() const { return _data; }

		template<typename T>
		bool try_get(T& value) const
		{
			if (const auto ptr = std::get_if<T>(&_data))
			{
				value = *ptr;
				return true;
			}

			return false;
		}

		template<typename T>
		T get(T default_value = {}) const
		{
			T value;
			return try_get(value) ? value : default_value;
		}

		// INTEGRAL //////////////////////////////////////////////////////////////////
		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		bool try_get_integral(T& value) const
		{
			if (const auto ptr = std::get_if<Int>(&_data))
			{
				value = static_cast<T>(*ptr);
				return true;
			}

			if (const auto ptr = std::get_if<Int64>(&_data))
			{
				value = static_cast<T>(*ptr);
				return true;
			}

			return false;
		}

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		T get_integral(T default_value = {}) const
		{
			T value;
			return try_get_integral(value) ? value : default_value;
		}

		// INTEGRAL //////////////////////////////////////////////////////////////////
		template<typename T,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		bool try_get_floating_point(T& value) const
		{
			if (const auto ptr = std::get_if<Float>(&_data))
			{
				value = static_cast<T>(*ptr);
				return true;
			}

			if (const auto ptr = std::get_if<Double>(&_data))
			{
				value = static_cast<T>(*ptr);
				return true;
			}

			return false;
		}

		template<typename T,
			std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
		T get_floating_point(T default_value = {}) const
		{
			T value;
			return try_get_floating_point(value) ? value : default_value;
		}

		// ENUM //////////////////////////////////////////////////////////////////////
		template<typename T,
			std::enable_if_t<std::is_enum_v<T>>* = nullptr>
		bool try_get_enum(T& value) const
		{
			using Type = std::underlying_type_t<T>;

			Type i;
			if (try_get_integral(i))
			{
				value = static_cast<T>(i);
				return true;
			}

			return false;
		}

		template<typename T,
			std::enable_if_t<std::is_enum_v<T>>* = nullptr>
		T get_enum(T default_value = static_cast<T>(0)) const
		{
			T value;
			return try_get_enum(value) ? value : default_value;
		}

		// BOOL ////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_bool(Bool& value) const;
		UC_NODISCARD Bool get_bool(Bool default_value = false) const;

		// INT_TYPE ////////////////////////////////////////////////////////////////
		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		UC_NODISCARD bool try_get_int_type(T& value) const
		{
			if (try_get_integral(value))
				return true;

			Double d;
			if (try_get_floating_point(d))
			{
				value = static_cast<T>(d);
				return true;
			}

			return false;
		}

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		UC_NODISCARD T get_int_type(T default_value = 0) const
		{
			T value;
			return try_get_int_type<T>(value) ? value : default_value;
		}

		// INT /////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_int(Int& value) const;
		UC_NODISCARD Int get_int(Int default_value = 0) const;

		// INT64 ///////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_int64(Int64& value) const;
		UC_NODISCARD Int64 get_int64(Int64 default_value = 0) const;

		// FLOAT ///////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_float(Float& value) const;
		UC_NODISCARD Float get_float(Float default_value = 0) const;

		// DOUBLE //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_double(Double& value) const;
		UC_NODISCARD Double get_double(Double default_value = 0) const;

		// STRING //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_string(String& value) const;
		UC_NODISCARD String get_string(StringView default_value = "") const;

		// STRING32 ////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_string32(String32& value) const;
		UC_NODISCARD String32 get_string32(StringView32 default_value = U"") const;

		// VECTOR2 /////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_vector2(Vector2<T>& value) const
		{
			if (const auto ptr = std::get_if<Vector2i>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Vector2f>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		UC_NODISCARD Vector2<T> get_vector2(
			const Vector2<T>& default_value = details::VectorConst2<T>::Zero) const
		{
			Vector2<T> value{};
			return try_get_vector2(value) ? value : default_value;
		}

		// VECTOR2I ////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec2i(Vector2i& value) const { return try_get_vector2(value); }
		UC_NODISCARD Vector2i get_vec2i(const Vector2i& default_value = VectorConst2i::Zero) const { return get_vector2(default_value); }

		// VECTOR2F //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec2f(Vector2f& value) const { return try_get_vector2(value); }
		UC_NODISCARD Vector2f get_vec2f(const Vector2f& default_value = VectorConst2f::Zero) const { return get_vector2(default_value); }

		// VECTOR3 /////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_vec3(Vector3<T>& value) const
		{
			if (const auto ptr = std::get_if<Vector3i>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Vector3f>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			// TODO: Cast from vector2?
			//Vector2<T> vec2;
			//if (try_get_vector2(vec2))
			//{
			//	value.set(vec2.x, vec2.y, 0);
			//	return true;
			//}

			return false;
		}

		template<typename T>
		UC_NODISCARD Vector3<T> get_vec3(
			const Vector3<T>& default_value = details::VectorConst3<T>::Zero) const
		{
			Vector3<T> value{};
			return try_get_vec3(value) ? value : default_value;
		}

		// VECTOR3I ////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec3i(Vector3i& value) const { return try_get_vec3(value); }
		UC_NODISCARD Vector3i get_vec3i(const Vector3i& default_value = VectorConst3i::Zero) const { return get_vec3(default_value); }

		// VECTOR3F ////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec3f(Vector3f& value) const { return try_get_vec3(value); }
		UC_NODISCARD Vector3f get_vec3f(const Vector3f& default_value = VectorConst3f::Zero) const { return get_vec3(default_value); }

		// RANGE ///////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_range(Range<T>& value) const
		{
			if (const auto ptr = std::get_if<Rangei>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Rangef>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		UC_NODISCARD Range<T> get_range(
			const Range<T>& default_value = details::RangeConst<T>::Zero) const
		{
			Range<T> value{};
			return try_get_range(value) ? value : default_value;
		}

		// RANGEI //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_rangei(Rangei& value) const { return try_get_range(value); }
		UC_NODISCARD Rangei get_rangei(const Rangei& default_value = RangeConsti::Zero) const { return get_range(default_value); }

		// RANGEF //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_rangef(Rangef& value) const { return try_get_range(value); }
		UC_NODISCARD Rangef get_rangef(const Rangef& default_value = RangeConstf::Zero) const { return get_range(default_value); }

		// RECT ////////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_rect(Rect<T>& value) const
		{
			if (const auto ptr = std::get_if<Recti>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Rectf>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			return false;
		}

		template<typename T>
		UC_NODISCARD Rect<T> get_rect(
			const Rect<T>& default_value = details::RectConst<T>::Zero) const
		{
			Rect<T> value{};
			return try_get_rect(value) ? value : default_value;
		}

		// RECTI ///////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_recti(Recti& value) const { return try_get_rect(value); }
		UC_NODISCARD Recti get_recti(const Recti& default_value = RectConsti::Zero) const { return get_rect(default_value); }

		// RECTF ///////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_rectf(Rectf& value) const { return try_get_rect(value); }
		UC_NODISCARD Rectf get_rectf(const Rectf& default_value = RectConstf::Zero) const { return get_rect(default_value); }

		// COLOR3 //////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_color3(Color3<T>& value) const
		{
			if (const auto ptr = std::get_if<Color3b>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Color3f>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			uint32_t i;
			if (try_get_integral(i))
			{
				if (i > 0xFF000000)
					value = Color3b::from_rgb(i >> 16).cast<T>();
				else
					value = Color3b::from_rgb(i).cast<T>();
				return true;
			}

			// TODO: Cast from Vector3?

			return false;
		}

		template<typename T>
		UC_NODISCARD Color3<T> get_color3(
			const Color3<T>& default_value = ColorTable<T>::Clear) const
		{
			Color3<T> value{};
			return try_get_color3(value) ? value : default_value;
		}

		// COLOR3B /////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_color3b(Color3b& value) const { return try_get_color3(value); }
		UC_NODISCARD Color3b get_color3b(const Color3b& default_value = ColorConst3b::White) const { return get_color3(default_value); }

		// COLOR3F /////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_color3f(Color3f& value) const { return try_get_color3(value); }
		UC_NODISCARD Color3f get_color3f(const Color3f& default_value = ColorConst3f::White) const { return get_color3(default_value); }

		// COLOR4 //////////////////////////////////////////////////////////////////
		template<typename T>
		bool try_get_color4(Color4<T>& value) const
		{
			if (const auto ptr = std::get_if<Color4b>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			if (const auto ptr = std::get_if<Color4f>(&_data))
			{
				value = ptr->cast<T>();
				return true;
			}

			uint32_t i;
			if (try_get_integral(i))
			{
				if (i > 0xFF000000)
					value = Color4b::from_argb(i).cast<T>();
				else
					value = Color4b::from_rgb(i).cast<T>();
				return true;
			}

			// TODO: Cast from Vector3?
			// TODO: Cast from Vector4?

			return false;
		}

		template<typename T>
		UC_NODISCARD Color4<T> get_color4(
			const Color4<T>& default_value = ColorTable<T>::Clear) const
		{
			Color4<T> value{};
			return try_get_color4(value) ? value : default_value;
		}

		// COLOR4B /////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_color4b(Color4b& value) const { return try_get_color4(value); }
		UC_NODISCARD Color4b get_color4b(const Color4b& default_value = ColorConst4b::White) const { return get_color4(default_value); }

		// COLOR4F /////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_color4f(Color4f& value) const { return try_get_color4(value); }
		UC_NODISCARD Color4f get_color4f(const Color4f& default_value = ColorConst4f::White) const { return get_color4(default_value); }

		// OBJECT //////////////////////////////////////////////////////////////////
		bool try_get_object(Shared<Object>& value) const;
		UC_NODISCARD Shared<Object> get_object(const Shared<Object>& default_value = nullptr) const;

		template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
		bool try_get_object_cast(Shared<T>& value) const
		{
			if (const auto ptr = std::get_if<Shared<Object>>(&_data))
			{
				auto converted = std::dynamic_pointer_cast<T>(*ptr);
				if (converted)
				{
					value = converted;
					return true;
				}
			}

			return false;
		}

		template<typename T, std::enable_if_t<std::is_base_of_v<Object, T>>* = nullptr>
		UC_NODISCARD Shared<T> get_object_cast(const Shared<T>& default_value = nullptr) const
		{
			Shared<T> value;
			return try_get_object_cast(value) ? value : default_value;
		}

		static const Variant Empty;

	protected:
		DataType _data;
	};

	static constexpr bool operator==(const Variant& a, const Variant& b)
	{
		return a.data() == b.data();
	}

	static constexpr bool operator!=(const Variant& a, const Variant& b)
	{
		return a.data() != b.data();
	}

	static constexpr bool operator<=(const Variant& a, const Variant& b)
	{
		return a.data() <= b.data();
	}

	static constexpr bool operator>=(const Variant& a, const Variant& b)
	{
		return a.data() >= b.data();
	}

	static constexpr bool operator<(const Variant& a, const Variant& b)
	{
		return a.data() <= b.data();
	}

	static constexpr bool operator>(const Variant& a, const Variant& b)
	{
		return a.data() >= b.data();
	}

	extern UNICODE_STRING_BUILDER_FORMAT(const Variant&);
}