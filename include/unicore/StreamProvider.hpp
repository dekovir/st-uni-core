#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Object.hpp"
#include "unicore/Path.hpp"

namespace unicore
{
	class MemoryChunk;
	class ReadStream;
	class WriteStream;

	enum class FileFlag : uint8_t
	{
		None = 0,
		Directory = 1,
		File = 2,
	};
	UNICORE_ENUMFLAGS(FileFlag, FileFlags);

	struct StreamStats
	{
		int64_t size = 0;
		// TODO: Replace with DateTime
		time_t modtime = 0;
		// TODO: Replace with DateTime
		time_t createtime = 0;
		// TODO: Replace with DateTime
		time_t accestime = 0;
		FileFlag flag = FileFlag::None;
	};

	class StreamProvider : public Object
	{
	public:
		UC_NODISCARD virtual Optional<StreamStats> stats(const Path& path) const = 0;
		UC_NODISCARD virtual bool exists(const Path& path) const;

		virtual uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const = 0;

		UC_NODISCARD virtual List<Path> get_enumerate(
			const Path& path, WStringView search_pattern,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const;

		virtual bool create_directory(const Path& path) = 0;
		virtual bool delete_directory(const Path& path, bool recursive = false) = 0;

		virtual Shared<ReadStream> open_read(const Path& path) = 0;
		virtual Shared<WriteStream> create_new(const Path& path) = 0;

		virtual bool delete_file(const Path& path) = 0;

		virtual Shared<MemoryChunk> read_chunk(const Path& path);
		virtual bool write_chunk(const Path& path, const MemoryChunk& chunk);
	};

	class PathStreamProvider : StreamProvider
	{
	public:
		explicit PathStreamProvider(StreamProvider& provider, const Path& base)
			: _provider(provider), _base(base) {}

		UC_NODISCARD Optional<StreamStats> stats(const Path& path) const override;
		UC_NODISCARD bool exists(const Path& path) const override;

		uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const override;

		bool create_directory(const Path& path) override;
		bool delete_directory(const Path& path, bool recursive) override;

		Shared<ReadStream> open_read(const Path& path) override;
		Shared<WriteStream> create_new(const Path& path) override;

		bool delete_file(const Path& path) override;

	protected:
		StreamProvider& _provider;
		Path _base;

		UC_NODISCARD Path make_path(const Path& path) const { return _base / path; }
	};
}