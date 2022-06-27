#pragma once
#include "unicore/Object.hpp"
#include "unicore/FileStats.hpp"
#include "unicore/Stream.hpp"
#include "unicore/Path.hpp"

namespace unicore
{
	class StreamProvider : public Object
	{
	public:
		UC_NODISCARD virtual Optional<FileStats> stats(const Path& path) const = 0;
		UC_NODISCARD virtual bool exists(const Path& path) const;

		virtual uint16_t enumerate(
			const Path& path, List<WString>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) = 0;

		virtual Shared<ReadStream> open_read(const Path& path) = 0;
		virtual Shared<WriteStream> create_new(const Path& path) = 0;
	};

	class PathStreamProvider : StreamProvider
	{
	public:
		explicit PathStreamProvider(StreamProvider& provider, const Path& base)
			: _provider(provider), _base(base) {}

		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;
		UC_NODISCARD bool exists(const Path& path) const override;

		uint16_t enumerate(const Path& path, List<WString>& name_list, FileFlags flags) override;
		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;

	protected:
		StreamProvider& _provider;
		Path _base;

		UC_NODISCARD Path make_path(const Path& path) const { return _base / path; }
	};
}