#include "unicore/szip/SZipFileProvider.hpp"
#include "unicore/Unicode.hpp"
#include "unicore/Logger.hpp"
#include "unicore/MemoryFile.hpp"
#include "SZipUtils.hpp"
#include "SZipError.hpp"

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
		List<UInt16> name_buffer;

		for (UInt32 index = 0; index < _data->db.NumFiles; index++)
		{
			const auto lng = SzArEx_GetFileNameUtf16(&_data->db, index, nullptr);

			name_buffer.resize(lng);
			SzArEx_GetFileNameUtf16(&_data->db, index, name_buffer.data());
			auto name_wcs = Unicode::to_wcs(BasicStringView<char16_t>(
				reinterpret_cast<char16_t*>(name_buffer.data()), name_buffer.size()));
			const Path path(name_wcs);

			add_entry(path, index);
		}

		UC_LOG_DEBUG(_logger) << "Cached " << _data->db.NumFiles << " files";
	}
}