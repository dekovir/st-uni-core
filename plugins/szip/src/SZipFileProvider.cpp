#include "unicore/szip/SZipFileProvider.hpp"
#include "unicore/system/Unicode.hpp"
#include "unicore/io/Logger.hpp"
#include "unicore/io/MemoryFile.hpp"
#include "unicore/szip/SZipUtils.hpp"
#include "unicore/szip/SZipError.hpp"

namespace unicore
{
	SZipFileProvider::~SZipFileProvider()
	{
		SzArEx_Free(&_data->db, SZipUtils::get_alloc_main());
	}

	// ============================================================================
	Optional<FileStats> SZipFileProvider::stats_index(intptr_t index) const
	{
		FileStats stats;
		if (SzArEx_IsDir(&_data->db, index))
		{
			stats.type = FileType::Directory;
		}
		else
		{
			stats.type = FileType::File;
			stats.size = SzArEx_GetFileSize(&_data->db, index);
		}

		return stats;
	}

	Shared<ReadFile> SZipFileProvider::open_read_index(intptr_t index)
	{
		size_t offset;
		size_t file_size;

		const auto result = SzArEx_Extract(&_data->db, &_data->look_stream.s, (UInt32)index,
			&_block_index, &_out_buffer, &_out_buffer_size, &offset, &file_size,
			SZipUtils::get_alloc_main(), SZipUtils::get_alloc_temp());
		if (result != SZ_OK)
		{
			UC_LOG_ERROR(_logger) << SZipError(result);
			return nullptr;
		}

		auto chunk = std::make_shared<MemoryChunk>(file_size);
		Memory::copy(chunk->data(), _out_buffer + offset, file_size);
		return std::make_shared<ReadMemoryFile>(chunk);
	}

	void SZipFileProvider::cache_files()
	{
		String16 name_buffer;

		for (UInt32 index = 0; index < _data->db.NumFiles; index++)
		{
			const auto lng = SzArEx_GetFileNameUtf16(&_data->db, index, nullptr);

			name_buffer.resize(lng);
			SzArEx_GetFileNameUtf16(&_data->db, index,
				reinterpret_cast<UInt16*>(name_buffer.data()));
			const Path path(name_buffer);

			add_entry(path, index);
		}

		UC_LOG_DEBUG(_logger) << "Cached " << _data->db.NumFiles << " files";
	}
}