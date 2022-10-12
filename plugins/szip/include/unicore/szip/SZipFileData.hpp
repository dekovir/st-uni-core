#pragma once
#include "unicore/io/File.hpp"
#include <7z.h>

namespace unicore
{
	class SZipStream : public ISeekInStream
	{
	public:
		int code = 42;
		Shared<ReadFile> file;

		explicit SZipStream(const Shared<ReadFile>& file);

		static SeekMethod convert(ESzSeek method);
		static SRes read(void* ptr, void* buf, size_t* size);
		static SRes seek(void* ptr, Int64* pos, ESzSeek origin);
	};

	class SZipFileData
	{
	public:
		CSzArEx db;
		CLookToRead look_stream;
		Shared<SZipStream> stream;

		explicit SZipFileData(const Shared<SZipStream>& stream);
	};
}