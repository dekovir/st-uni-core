#include "unicore/system/Debug.hpp"
#include "unicore/system/StringBuilder.hpp"

namespace unicore
{
	UNICODE_STRING_BUILDER_FORMAT(const DebugSource&)
	{
		// TODO: Replace with constexpr
#if defined (UNICORE_PLATFORM_WINDOWS) || defined(UNICORE_PLATFORM_ANDROID)
		const auto* pos = strrchr(value.file.data(), '\\');
#else
		const auto* pos = strrchr(value.file.data(), '/');
#endif
		if (pos != nullptr)
			builder.append(pos + 1);
		else builder.append(value.file);
		return builder << ":" << value.line;
	}

	UNICODE_STRING_BUILDER_FORMAT(const DebugFunction&)
	{
		return builder << value.name;
	}
}