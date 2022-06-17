#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	template<typename T>
	class EnumFlag
	{
		static_assert(std::is_enum_v<T>, "Must be enum");
	public:
		typedef std::underlying_type_t<T> TValue;

		EnumFlag() : _value(0) {}
		EnumFlag(T value) : _value(static_cast<TValue>(value)) {}
		explicit EnumFlag(TValue value) : _value(value) {}

		UC_NODISCARD bool empty() const { return _value == 0; }

		UC_NODISCARD bool has(T flag) const
		{
			return (_value & static_cast<TValue>(flag)) == static_cast<TValue>(flag);
		}

		void set(T flag)
		{
			_value |= static_cast<TValue>(flag);
		}

		void remove(T flag)
		{
			_value &= ~static_cast<TValue>(flag);
		}

		void set(T flag, bool value)
		{
			if (value)
				set(flag);
			else remove(flag);
		}

		void toggle(T flag)
		{
			if (flag(flag))
				remove(flag);
			else set(flag);
		}

		bool operator[](T flag) const { return flag(flag); }

		EnumFlag<T> operator|(T flag) const
		{
			EnumFlag<T> flags(_value);
			flags.set(flag);
			return flags;
		}

		EnumFlag<T>& operator|=(T flag)
		{
			set(flag);
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

		static bool is_changed(const EnumFlag<T>& a, const EnumFlag<T>& b, T flag)
		{
			return a.has(flag) != b.has(flag);
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