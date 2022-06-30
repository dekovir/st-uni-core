#pragma once
#include "unicore/Defs.hpp"
#include <chrono>

namespace unicore
{
	class TimeSpan
	{
	public:
		constexpr TimeSpan() : _data(0) {}
		explicit constexpr TimeSpan(std::chrono::nanoseconds value) : _data(value) {}

		// Copy constructor
		constexpr TimeSpan(const TimeSpan& other) = default;

		// Move constructor
		constexpr TimeSpan(TimeSpan&& other) noexcept
			: _data(other._data) {}

		~TimeSpan() = default;

		// Copy assignment operator
		TimeSpan& operator=(const TimeSpan& other) noexcept = default;

		// Move assignment operator
		TimeSpan& operator=(TimeSpan&& other) noexcept
		{
			_data = other._data;
			return *this;
		}

		UC_NODISCARD constexpr auto data() const { return _data; }

		UC_NODISCARD constexpr TimeSpan add(const TimeSpan& other) const
		{
			return TimeSpan(_data + other._data);
		}

		UC_NODISCARD constexpr TimeSpan sub(const TimeSpan& other) const
		{
			return TimeSpan(_data + other._data);
		}

		UC_NODISCARD constexpr uint64_t total_milliseconds() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(_data).count();
		}

		UC_NODISCARD constexpr double total_seconds() const
		{
			return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(_data).count()) / 1000;
		}

		TimeSpan& operator += (const TimeSpan& ts)
		{
			_data += ts._data;
			return *this;
		}

		TimeSpan& operator -= (const TimeSpan& other)
		{
			_data -= other._data;
			return *this;
		}

		template<typename TRep, typename TPeriod>
		static  constexpr TimeSpan from_duration(std::chrono::duration<TRep, TPeriod> value)
		{
			return TimeSpan(std::chrono::duration_cast<decltype(_data)>(value));
		}

		static constexpr TimeSpan from_microseconds(uint64_t ms)
		{
			return TimeSpan(std::chrono::duration<uint64_t, std::micro>(ms));
		}

		static constexpr TimeSpan from_milliseconds(uint64_t ms)
		{
			return TimeSpan(std::chrono::duration<uint64_t, std::milli>(ms));
		}

		static constexpr TimeSpan from_seconds(float value)
		{
			return from_duration(std::chrono::duration<float>(value));
		}

		static constexpr TimeSpan from_seconds(double value)
		{
			return from_duration(std::chrono::duration<double>(value));
		}

		static constexpr TimeSpan from_minutes(double value)
		{
			return from_duration(std::chrono::duration<double, std::ratio<60>>(value));
		}

		static constexpr TimeSpan from_hours(double value)
		{
			return from_duration(std::chrono::duration<double, std::ratio<3600>>(value));
		}

	protected:
		std::chrono::nanoseconds _data;
	};

	static constexpr bool operator==(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() == b.data();
	}

	static constexpr bool operator!=(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() != b.data();
	}

	static constexpr bool operator<=(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() <= b.data();
	}

	static constexpr bool operator>=(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() >= b.data();
	}

	static constexpr bool operator<(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() < b.data();
	}

	static constexpr bool operator>(const TimeSpan& a, const TimeSpan& b)
	{
		return a.data() > b.data();
	}

	static constexpr TimeSpan operator +(const TimeSpan& a, const TimeSpan& b)
	{
		return a.add(b);
	}

	static constexpr TimeSpan operator -(const TimeSpan& a, const TimeSpan& b)
	{
		return a.sub(b);
	}

	struct TimeSpanConst
	{
		static constexpr TimeSpan Zero = TimeSpan::from_milliseconds(0);
		static constexpr TimeSpan OneSecond = TimeSpan::from_seconds(1.f);
	};
}