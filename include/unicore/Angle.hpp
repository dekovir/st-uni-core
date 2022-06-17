#pragma once
#include "unicore/Defs.hpp"
#include "unicore/Math.hpp"

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

		UC_NODISCARD constexpr float value() const { return _value; }

		UC_NODISCARD inline float cos() const { return TypeTag::cos(_value); }
		UC_NODISCARD inline float sin() const { return TypeTag::sin(_value); }

		template<typename OtherTag>
		constexpr Angle<OtherTag> cast() const;

	protected:
		float _value;
	};

	template <typename TypeTag>
	template <typename OtherTag>
	constexpr Angle<OtherTag> Angle<TypeTag>::cast() const
	{
		static_assert(true);
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

	using Degrees = Angle<AngleTypeDeg>;
	using Radians = Angle<AngleTypeRad>;

	template<typename AngleTag>
	struct AngleConst
	{
		static constexpr auto Zero = Angle<AngleTag>(0);
		static constexpr auto Pi = Angle<AngleTag>(Angle<AngleTypeRad>(Math::PI));
		static constexpr auto PiHalf = Angle<AngleTag>(Angle<AngleTypeRad>(Math::PI / 2));
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
}