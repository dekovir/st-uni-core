#pragma once
#include "unicore/system/Debug.hpp"
#include <7z.h>

namespace unicore
{
	class SZipError : public Error<SRes>
	{
	public:
		explicit SZipError(int code);

		UC_NODISCARD StringView text() const;
	};

	extern UNICODE_STRING_BUILDER_FORMAT(const SZipError&);
}