#pragma once
#include "unicore/Module.hpp"
#include "unicore/io/FileProvider.hpp"

namespace unicore
{
	class FileSystem : public Module, public WriteFileProvider
	{
		UC_OBJECT(FileSystem, Module)
	public:
		explicit FileSystem(Logger& logger);

		void add_read(const Shared<ReadFileProvider>& provider);
		void set_write(const Shared<WriteFileProvider>& provider);

		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path, StringView search_pattern,
			List<String>& name_list, const EnumerateOptions& options) const override;

		UC_NODISCARD Shared<ReadFile> open_read(const Path& path) override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<WriteFile> create_new(const Path& path) override;
		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;
		List<Shared<ReadFileProvider>> _providers;
		Shared<WriteFileProvider> _write;
	};
}