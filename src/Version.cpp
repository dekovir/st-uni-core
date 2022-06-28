#include "unicore/Version.hpp"
#include "unicore/Logger.hpp"

namespace unicore
{
	Version::Version(uint16_t _major, uint16_t _minor)
		: major(_major), minor(_minor), build(0), revision(0)
	{
	}

	Version::Version(uint16_t _major, uint16_t _minor, uint16_t _build)
		: major(_major), minor(_minor), build(_build), revision(0)
	{
	}

	Version::Version(uint16_t _major, uint16_t _minor, uint16_t _build, uint16_t _revision)
		: major(_major), minor(_minor), build(_build), revision(_revision)
	{
	}

	bool Version::operator==(const Version& other) const
	{
		return data == other.data;
	}

	bool Version::operator!=(const Version& other) const
	{
		return data != other.data;
	}

	bool Version::operator<=(const Version& other) const
	{
		return data <= other.data;
	}

	bool Version::operator>=(const Version& other) const
	{
		return data >= other.data;
	}

	bool Version::operator<(const Version& other) const
	{
		return data < other.data;
	}

	bool Version::operator>(const Version& other) const
	{
		return data > other.data;
	}

	Version Version::parse(uint64_t value)
	{
		return {
				static_cast<uint16_t>((value >> 48) & 0xFFFF),
				static_cast<uint16_t>((value >> 32) & 0xFFFF),
				static_cast<uint16_t>((value >> 16) & 0xFFFF),
				static_cast<uint16_t>(value & 0xFFFF)
		};
	}

	LogHelper& operator << (LogHelper& helper, const Version& ver)
	{
		helper << ver.major << L"." << ver.minor;

		if (ver.build > 0 || ver.revision > 0)
			helper << L"." << ver.build;
		if (ver.revision > 0)
			helper << L"." << ver.revision;

		return helper;
	}
}