#include "unicore/szip/SZipFileData.hpp"

namespace unicore
{
	SZipStream::SZipStream(const Shared<ReadFile>& file)
		: ISeekInStream({ &read, &seek }), file(file)
	{
	}

	SeekMethod SZipStream::convert(ESzSeek method)
	{
		switch (method)
		{
		case SZ_SEEK_CUR: return SeekMethod::Current;
		case SZ_SEEK_END: return SeekMethod::End;
		default: return SeekMethod::Begin;
		}
	}

	SRes SZipStream::read(void* ptr, void* buf, size_t* size)
	{
		const auto data = static_cast<SZipStream*>(ptr);

		if (data->file->read(buf, *size, size))
			return SZ_OK;

		return SZ_ERROR_READ;
	}

	SRes SZipStream::seek(void* ptr, Int64* pos, ESzSeek origin)
	{
		const auto data = static_cast<SZipStream*>(ptr);
		*pos = data->file->seek(*pos, convert(origin));
		return SZ_OK;
	}

	SZipFileData::SZipFileData(const Shared<SZipStream>& stream)
		: db({})
		, stream(stream)
	{
		LookToRead_CreateVTable(&look_stream, 0);
		look_stream.realStream = stream.get();
		LookToRead_Init(&look_stream);
	}
}