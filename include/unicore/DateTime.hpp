#pragma once
#include "unicore/TimeSpan.hpp"

namespace unicore
{
	class DateTime
	{
	public:
		using ClockType = std::chrono::system_clock;
		using DataType = std::chrono::time_point<ClockType>;

		constexpr DateTime() = default;

		explicit constexpr DateTime(const DataType& data) noexcept
			: _data(data) {}

		explicit DateTime(time_t data) noexcept;

		template<typename Duration>
		explicit constexpr DateTime(const std::chrono::time_point<ClockType, Duration>& data)
			: _data(std::chrono::time_point_cast<ClockType::duration>(data))
		{
		}

		UC_NODISCARD constexpr DataType data() const { return _data; }

		UC_NODISCARD time_t to_time_t() const noexcept;

		static DateTime now() noexcept;

		template<typename Duration>
		static constexpr DateTime from_duration(const std::chrono::time_point<ClockType, Duration>& duration)
		{
			const DataType data = duration;
			return DateTime(data);
		}

	protected:
		DataType _data;
	};

	// COMPARE OPERATORS /////////////////////////////////////////////////////////
	static constexpr bool operator==(const DateTime& a, const DateTime& b)
	{
		return a.data() == b.data();
	}

	static constexpr bool operator!=(const DateTime& a, const DateTime& b)
	{
		return a.data() != b.data();
	}

	static constexpr bool operator>=(const DateTime& a, const DateTime& b)
	{
		return a.data() >= b.data();
	}

	static constexpr bool operator<=(const DateTime& a, const DateTime& b)
	{
		return a.data() <= b.data();
	}

	static constexpr bool operator<(const DateTime& a, const DateTime& b)
	{
		return a.data() < b.data();
	}

	static constexpr bool operator>(const DateTime& a, const DateTime& b)
	{
		return a.data() > b.data();
	}

	// OPERATORS WITH TIMESPAN ///////////////////////////////////////////////////
	static constexpr DateTime operator+(const DateTime& time, const TimeSpan& delta)
	{
		return DateTime(time.data() + delta.data());
	}

	static constexpr DateTime operator-(const DateTime& time, const TimeSpan& delta)
	{
		return DateTime(time.data() - delta.data());
	}

	// ARITHMETIC OPERATORS //////////////////////////////////////////////////////
	static constexpr TimeSpan operator-(const DateTime& a, const DateTime& b)
	{
		const auto duration = a.data() - b.data();
		return TimeSpan::from_duration(duration);
	}

	extern UNICODE_STRING_BUILDER_FORMAT(const DateTime&);
}