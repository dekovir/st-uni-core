#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename T>
	class EnumFlag
	{
		static_assert(std::is_enum<T>::value, "Must be enum");
	public:
		typedef typename std::underlying_type<T>::type TValue;

		EnumFlag() : _value(0) {}
		EnumFlag(T value) : _value(static_cast<TValue>(value)) {}
		explicit EnumFlag(TValue value) : _value(value) {}

		bool IsFlag(T flag) const
		{
			return (_value & static_cast<TValue>(flag)) == static_cast<TValue>(flag);
		}

		bool HasFlag(T flag) const
		{
			return (_value & static_cast<TValue>(flag)) == static_cast<TValue>(flag);
		}

		void SetFlag(T flag)
		{
			_value |= static_cast<TValue>(flag);
		}

		void RemoveFlag(T flag)
		{
			_value &= ~static_cast<TValue>(flag);
		}

		void SetFlag(T flag, bool value)
		{
			if (value)
				SetFlag(flag);
			else RemoveFlag(flag);
		}

		bool IsEmpty() const { return _value == 0; }

		bool operator[](T flag) const { return HasFlag(flag); }

		EnumFlag<T> operator|(T flag) const
		{
			EnumFlag<T> flags(_value);
			flags.SetFlag(flag);
			return flags;
		}

		EnumFlag<T>& operator|=(T flag)
		{
			SetFlag(flag);
			return *this;
		}

		EnumFlag<T>& operator=(T flag)
		{
			_value = static_cast<TValue>(flag);
			return *this;
		}

		EnumFlag<T>& operator=(TValue value)
		{
			_value = value;
			return *this;
		}

		bool operator == (const EnumFlag<T>& other) const
		{
			return _value == other._value;
		}

		bool operator != (const EnumFlag<T>& other) const
		{
			return _value != other._value;
		}

		static bool IsFlagChanged(const EnumFlag<T>& a, const EnumFlag<T>& b, T flag)
		{
			return a.IsFlag(flag) != b.IsFlag(flag);
		}

		static const EnumFlag<T> Zero;

	protected:
		TValue _value;
	};

	template<typename T>
	const EnumFlag<T> EnumFlag<T>::Zero(0);

#define UNICORE_ENUMFLAGS(TFlag, TName) \
		typedef EnumFlag<TFlag> TName; \
		inline TName operator|(TFlag a, TFlag b) { \
				return TName(static_cast<TName::TValue>(a) | static_cast<TName::TValue>(b)); \
		}
}