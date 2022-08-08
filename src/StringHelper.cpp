#include "unicore/StringHelper.hpp"

namespace unicore::StringHelper
{
	int compare(const StringView a, const StringView b, bool case_sensitive)
	{
		if (case_sensitive)
			return strcmp(a.data(), b.data());

#if defined(UNICORE_PLATFORM_WINDOWS)
		return _stricmp(a.data(), b.data());
#else
		return strcasecmp(a.data(), b.data());
#endif
	}

	int compare(const WStringView a, const WStringView b, bool case_sensitive)
	{
		if (case_sensitive)
			return wcscmp(a.data(), b.data());

#if defined(UNICORE_PLATFORM_WINDOWS)
		return _wcsicmp(a.data(), b.data());
#else
		return wcscasecmp(a.data(), b.data());
#endif
	}
}