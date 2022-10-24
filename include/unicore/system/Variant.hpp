#pragma once
#include "unicore/math/Rect.hpp"
#include "unicore/math/Vector3.hpp"
#include "unicore/renderer/Color3.hpp"
#include "unicore/renderer/Color4.hpp"

namespace unicore
{
	class Variant
	{
	public:
		using DataType = StdVariant<
			Bool, Int, Int64,
			Float, Double,
			String, String32,
			Vector2i, Vector2f, Vector3i, Vector3f, Recti, Rectf,
			Color3b, Color3f, Color4b, Color4f
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

		// BOOL //////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_bool(Bool& value) const;
		UC_NODISCARD Bool get_bool(Bool default_value = false) const;

		// INT ///////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_int(Int& value) const;
		UC_NODISCARD Int get_int(Int default_value = 0) const;

		// INT64 /////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_int64(Int64& value) const;
		UC_NODISCARD Int64 get_int64(Int64 default_value = 0) const;

		// FLOAT /////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_float(Float& value) const;
		UC_NODISCARD Float get_float(Float default_value = 0) const;

		// DOUBLE ////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_double(Double& value) const;
		UC_NODISCARD Double get_double(Double default_value = 0) const;

		// STRING ////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_string(String& value) const;
		UC_NODISCARD String get_string(StringView default_value = "") const;

		// STRING32 //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_string32(String32& value) const;
		UC_NODISCARD String32 get_string32(StringView32 default_value = U"") const;

		// VECTOR2 ///////////////////////////////////////////////////////////////////
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

		// VECTOR2I //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec2i(Vector2i& value) const { return try_get_vector2(value); }
		UC_NODISCARD Vector2i get_vec2i(const Vector2i& default_value = VectorConst2i::Zero) const { return get_vector2(default_value); }

		// VECTOR2F //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec2f(Vector2f& value) const { return try_get_vector2(value); }
		UC_NODISCARD Vector2f get_vec2f(const Vector2f& default_value = VectorConst2f::Zero) const { return get_vector2(default_value); }

		// VECTOR3 ///////////////////////////////////////////////////////////////////
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

		// VECTOR3I //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec3i(Vector3i& value) const { return try_get_vec3(value); }
		UC_NODISCARD Vector3i get_vec3i(const Vector3i& default_value = VectorConst3i::Zero) const { return get_vec3(default_value); }

		// VECTOR3F //////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_vec3f(Vector3f& value) const { return try_get_vec3(value); }
		UC_NODISCARD Vector3f get_vec3f(const Vector3f& default_value = VectorConst3f::Zero) const { return get_vec3(default_value); }

		// RECT //////////////////////////////////////////////////////////////////////
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

		// RECTI /////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_recti(Recti& value) const { return try_get_rect(value); }
		UC_NODISCARD Recti get_recti(const Recti& default_value = RectConsti::Zero) const { return get_rect(default_value); }

		// RECTF /////////////////////////////////////////////////////////////////////
		UC_NODISCARD bool try_get_rectf(Rectf& value) const { return try_get_rect(value); }
		UC_NODISCARD Rectf get_rectf(const Rectf& default_value = RectConstf::Zero) const { return get_rect(default_value); }

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