#pragma once
#include "unicore/io/FileProvider.hpp"
#if defined(UNICORE_PLATFORM_POSIX)

namespace unicore
{
	class PosixFileProvider : public WriteFileProvider
	{
		UC_OBJECT(PosixFileProvider, WriteFileProvider)
	public:
		explicit PosixFileProvider(Logger& logger);
		PosixFileProvider(Logger& logger, const Path& current_dir);

		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path,
			StringView search_pattern, List<String>& name_list,
			const EnumerateOptions& options) const override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<ReadFile> open_read(const Path& path) override;
		Shared<WriteFile> create_new(const Path& path) override;

		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;
		Path _current_dir;

		UC_NODISCARD Path get_current_dir() const;
		UC_NODISCARD String to_native(const Path& path) const;
	};
}

#endif