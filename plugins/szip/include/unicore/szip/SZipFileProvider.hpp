#pragma once
#include "unicore/FileProvider.hpp"
#include "unicore/szip/SZipFileData.hpp"

namespace unicore
{
	class SZipFileProvider : public CachedFileProvider
	{
		UC_OBJECT(SZipFileProvider, CachedFileProvider)
	public:
		SZipFileProvider(Unique<SZipFileData> data, Logger* logger)
			: _data(std::move(data)), _logger(logger)
		{
			cache_files();
		}

		~SZipFileProvider() override;

	protected:
		Unique<SZipFileData> _data;
		Logger* _logger;

		// Cached values
		UInt32 _block_index = UINT32_MAX;
		Byte* _out_buffer = nullptr;
		size_t _out_buffer_size = 0;

		UC_NODISCARD Optional<FileStats> stats_index(intptr_t index) const override;
		UC_NODISCARD Shared<ReadFile> open_read_index(intptr_t index) override;

		void cache_files();
	};
}