#pragma once
#include "unicore/EnumFlag.hpp"

namespace unicore
{
	enum class FileFlag : uint8_t
	{
		None = 0,
		Directory = 1,
		File = 2,
	};
	UNICORE_ENUMFLAGS(FileFlag, FileFlags);

	struct FileStats
	{
		int64_t size = 0;
		// TODO: Replace with DateTime
		time_t modtime = 0;
		// TODO: Replace with DateTime
		time_t createtime = 0;
		// TODO: Replace with DateTime
		time_t accestime = 0;
		FileFlag flag = FileFlag::None;
	};
}