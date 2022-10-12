#include "unicore/Version.hpp"
#include "unicore/math/Math.hpp"
#include "unicore/io/Logger.hpp"

namespace unicore
{
	constexpr Version::Version(uint16_t _major, uint16_t _minor)
		: major(_major), minor(_minor), build(0), revision(0)
	{
	}

	constexpr Version::Version(uint16_t _major, uint16_t _minor, uint16_t _build)
		: major(_major), minor(_minor), build(_build), revision(0)
	{
	}

	constexpr Version::Version(uint16_t _major, uint16_t _minor, uint16_t _build, uint16_t _revision)
		: major(_major), minor(_minor), build(_build), revision(_revision)
	{
	}

	constexpr int Version::compare(const Version& other) const
	{
		auto ret = Math::compare(major, other.major);
		if (ret == 0) ret = Math::compare(minor, other.minor);
		if (ret == 0) ret = Math::compare(build, other.build);
		if (ret == 0) ret = Math::compare(revision, other.revision);
		return ret;
	}

	constexpr int Version::compare(const Version& a, const Version& b)
	{
		return a.compare(b);
	}

	constexpr Version Version::parse(uint64_t value)
	{
		return {
				static_cast<uint16_t>((value >> 48) & 0xFFFF),
				static_cast<uint16_t>((value >> 32) & 0xFFFF),
				static_cast<uint16_t>((value >> 16) & 0xFFFF),
				static_cast<uint16_t>(value & 0xFFFF)
		};
	}

	constexpr bool operator==(const Version& a, const Version& b)
	{
		return
			a.major == b.major &&
			a.minor == b.minor &&
			a.build == b.build &&
			a.revision == b.revision;
	}

	constexpr bool operator!=(const Version& a, const Version& b)
	{
		return !(a == b);
	}

	constexpr bool operator<=(const Version& a, const Version& b)
	{
		return Version::compare(a, b) <= 0;
	}

	constexpr bool operator>=(const Version& a, const Version& b)
	{
		return Version::compare(a, b) >= 0;
	}

	constexpr bool operator<(const Version& a, const Version& b)
	{
		return Version::compare(a, b) < 0;
	}

	constexpr bool operator>(const Version& a, const Version& b)
	{
		return Version::compare(a, b) > 0;
	}

	UNICODE_STRING_BUILDER_FORMAT(const Version&)
	{
		builder << value.major << L"." << value.minor;

		if (value.build > 0 || value.revision > 0)
			builder << L"." << value.build;
		if (value.revision > 0)
			builder << L"." << value.revision;

		return builder;
	}
}