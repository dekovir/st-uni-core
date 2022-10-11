#pragma once
#include "unicore/Math.hpp"
#include "unicore/Hash.hpp"

namespace unicore
{
	struct AngleTypeRad
	{
		static inline float cos(float value) { return Math::cos(value); }
		static inline float sin(float value) { return Math::sin(value); }
	};

	struct AngleTypeDeg
	{
		static inline float to_rad(float value) { return Math::DEG_TO_RAD * value; }
		static inline float cos(float value) { return Math::cos(to_rad(value)); }
		static inline float sin(float value) { return Math::sin(to_rad(value)); }
	};

	template<typename TypeTag>
	class Angle
	{
	public:
		constexpr Angle() : _value(0) {}
		explicit constexpr Angle(float value) : _value(value) {}

		template<typename OtherTag>
		constexpr Angle(const Angle<OtherTag>& other)
			: _value(other.template cast<TypeTag>().value()) {}

		constexpr Angle(const Angle& other) = default;
		constexpr Angle(Angle&& other) noexcept = default;

		~Angle() = default;

		Angle& operator=(const Angle& other) = default;
		Angle& operator=(Angle&& other) noexcept = default;

		UC_NODISCARD constexpr float value() const { return _value; }

		UC_NODISCARD float cos() const { return TypeTag::cos(_value); }
		UC_NODISCARD float sin() const { return TypeTag::sin(_value); }

		void sin_cos(float& sin_value, float& cos_value) const
		{
			sin_value = TypeTag::sin(_value);
			cos_value = TypeTag::cos(_value);
		}

		Angle& operator+=(const Angle& other)
		{
			_value += other.value();
			return *this;
		}

		Angle& operator-=(const Angle& other)
		{
			_value -= other.value();
			return *this;
		}

		constexpr Angle operator-() const
		{
			return Angle(-_value);
		}

		template<typename OtherTag>
		constexpr Angle<OtherTag> cast() const;

	protected:
		float _value;
	};

	template <typename TypeTag>
	template <typename OtherTag>
	constexpr Angle<OtherTag> Angle<TypeTag>::cast() const
	{
		UC_ASSERT_ALWAYS_MSG("Unimplemented cast");
	}

	template <>
	template <>
	constexpr Angle<AngleTypeRad> Angle<AngleTypeDeg>::cast() const
	{
		return Angle<AngleTypeRad>(Math::DEG_TO_RAD * _value);
	}

	template <>
	template <>
	constexpr Angle<AngleTypeDeg> Angle<AngleTypeRad>::cast() const
	{
		return Angle<AngleTypeDeg>(Math::RAD_TO_DEG * _value);
	}

	template<typename TypeTag>
	static constexpr bool operator==(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() == b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator!=(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() != b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator>(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() > b.value();
	}

	template<typename TypeTag>
	static constexpr bool operator<(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return a.value() < b.value();
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator+(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return Angle<TypeTag>(a.value() + b.value());
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator-(const Angle<TypeTag>& a, const Angle<TypeTag>& b)
	{
		return Angle<TypeTag>(a.value() - b.value());
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator*(const Angle<TypeTag>& a, float value)
	{
		return Angle<TypeTag>(a.value() * value);
	}

	template<typename TypeTag>
	static constexpr Angle<TypeTag> operator/(const Angle<TypeTag>& a, float value)
	{
		return Angle<TypeTag>(a.value() / value);
	}

	using Degrees = Angle<AngleTypeDeg>;
	using Radians = Angle<AngleTypeRad>;

	template<typename AngleTag>
	struct AngleConst
	{
		static constexpr auto Zero = Angle<AngleTag>(0);
		static constexpr auto Pi = Angle<AngleTag>(Angle<AngleTypeRad>(Math::Pi));
		static constexpr auto PiHalf = Angle<AngleTag>(Angle<AngleTypeRad>(Math::Pi / 2));
	};

	using DegreesConst = AngleConst<AngleTypeDeg>;
	using RadiansConst = AngleConst<AngleTypeRad>;

	static constexpr Radians operator"" _rad(unsigned long long value)
	{
		return Radians(static_cast<float>(value));
	}

	static constexpr Radians operator"" _rad(long double value)
	{
		return Radians(static_cast<float>(value));
	}

	static constexpr Degrees operator"" _deg(unsigned long long value)
	{
		return Degrees(static_cast<float>(value));
	}

	static constexpr Degrees operator"" _deg(long double value)
	{
		return Degrees(static_cast<float>(value));
	}

	UNICORE_MAKE_HASH(Radians)
	{
		return make(value.value());
	}

	UNICORE_MAKE_HASH(Degrees)
	{
		return make(value);
	}
}