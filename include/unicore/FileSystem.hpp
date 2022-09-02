#pragma once
#include "unicore/Module.hpp"
#include "unicore/FileProvider.hpp"

namespace unicore
{
	class FileProviderLoader;

	class FileSystem : public Module, public WriteFileProvider
	{
		UC_OBJECT(FileSystem, Module)
	public:
		explicit FileSystem(Logger& logger);

		void add_read(const Shared<ReadFileProvider>& provider);
		void set_write(const Shared<WriteFileProvider>& provider);

		void add_loader(const Shared<FileProviderLoader>& creator);

		bool mount(const Path& path);

		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path, WStringView search_pattern,
			List<WString>& name_list, const EnumerateOptions& options) const override;

		UC_NODISCARD Shared<ReadFile> open_read(const Path& path) override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<WriteFile> create_new(const Path& path) override;
		bool delete_file(const Path& path) override;

	protected:
		Logger& _logger;
		List<Shared<ReadFileProvider>> _providers;
		Shared<WriteFileProvider> _write;

		Set<Shared<FileProviderLoader>> _provider_loaders;
	};
}