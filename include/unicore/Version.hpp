#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Version
	{
	public:
		union
		{
			uint64_t data;
			struct
			{
				uint16_t major;
				uint16_t minor;
				uint16_t build;
				uint16_t revision;
			};
		};

		Version(uint16_t major, uint16_t minor);
		Version(uint16_t major, uint16_t minor, uint16_t build);
		Version(uint16_t major, uint16_t minor, uint16_t build, uint16_t revision);

		bool operator ==(const Version& other) const { return data == other.data; }
		bool operator !=(const Version& other) const { return data != other.data; }

		bool operator <=(const Version& other) const { return data <= other.data; }
		bool operator >=(const Version& other) const { return data >= other.data; }

		bool operator <(const Version& other) const { return data < other.data; }
		bool operator >(const Version& other) const { return data > other.data; }

		static Version parse(uint64_t value);
	};

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const Version& ver);
}
