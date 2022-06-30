#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Version
	{
	public:
		uint16_t major;
		uint16_t minor;
		uint16_t build;
		uint16_t revision;

		constexpr Version(uint16_t major, uint16_t minor);
		constexpr Version(uint16_t major, uint16_t minor, uint16_t build);
		constexpr Version(uint16_t major, uint16_t minor, uint16_t build, uint16_t revision);

		// Copy constructor
		constexpr Version(const Version& other) = default;

		// Move constructor
		constexpr Version(Version&& other) noexcept = default;

		~Version() = default;

		// Copy assignment operator
		Version& operator=(const Version& other) noexcept = default;

		// Move assignment operator
		Version& operator=(Version&& other) noexcept = default;

		UC_NODISCARD constexpr int compare(const Version& other) const;

		static constexpr int compare(const Version& a, const Version& b);

		static constexpr Version parse(uint64_t value);
	};

	static constexpr bool operator ==(const Version& a, const Version& b);
	static constexpr bool operator !=(const Version& a, const Version& b);

	static constexpr bool operator <=(const Version& a, const Version& b);
	static constexpr bool operator >=(const Version& a, const Version& b);

	static constexpr bool operator <(const Version& a, const Version& b);
	static constexpr bool operator >(const Version& a, const Version& b);

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const Version& ver);
}