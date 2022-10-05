#include "SZipError.hpp"
#include "unicore/StringBuilder.hpp"

namespace unicore
{
	SZipError::SZipError(int code)
		: Error<int>(code)
	{
	}

	StringView SZipError::text() const
	{
		switch (_code)
		{
		case SZ_OK: return "Ok";
		case SZ_ERROR_DATA: return "Data error";
		case SZ_ERROR_MEM: return "Memory error";
		case SZ_ERROR_CRC: return "CRC error";
		case SZ_ERROR_UNSUPPORTED: return "Unsupported error";
		case SZ_ERROR_PARAM: return "Parameter error";
		case SZ_ERROR_INPUT_EOF: return "Input EOF error";
		case SZ_ERROR_OUTPUT_EOF: return "Output EOF error";
		case SZ_ERROR_READ: return "Read error";
		case SZ_ERROR_WRITE: return "Write error";
		case SZ_ERROR_PROGRESS: return "Progress error";
		case SZ_ERROR_FAIL: return "Failed";
		case SZ_ERROR_THREAD: return "Thread error";
		case SZ_ERROR_ARCHIVE: return "Archive error";
		case SZ_ERROR_NO_ARCHIVE: return "No archive";
		default:
			return "Unknown";
		}
	}

	UNICODE_STRING_BUILDER_FORMAT(const SZipError&)
	{
		return builder << value.text() << ':' << value.code();
	}
}