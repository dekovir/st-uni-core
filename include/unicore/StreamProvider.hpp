#pragma once
#include "unicore/Object.hpp"
#include "unicore/FileStats.hpp"
#include "unicore/Stream.hpp"
#include "unicore/Path.hpp"

namespace unicore
{
	class BasicStreamProvider : public Object
	{
	public:
		UC_NODISCARD virtual Optional<FileStats> stats(const Path& path) = 0;
		UC_NODISCARD virtual bool exists(const Path& path);

		virtual uint16_t enumerate(
			const Path& path, List<Path>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) = 0;

		virtual Shared<ReadStream> open_read(const Path& path) = 0;
		virtual Shared<WriteStream> create_new(const Path& path) = 0;
	};

	class StreamProvider : public BasicStreamProvider
	{
	public:
	};
}