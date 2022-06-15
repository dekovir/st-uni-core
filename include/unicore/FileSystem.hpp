#pragma once
#include "unicore/Module.hpp"
#include "StreamProvider.hpp"

namespace unicore
{
	class FileSystem : public Module, public BasicStreamProvider
	{
	public:
		bool exists(const Path& path) override;
		Optional<FileStats> stats(const Path& path) override;
		
		uint16_t enumerate(
			const Path& path, List<Path>& name_list, FileFlags flags) override;

		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;

		void add_provider(const Shared<StreamProvider>& provider);

	protected:
		List<Shared<StreamProvider>> _providers;
	};
}