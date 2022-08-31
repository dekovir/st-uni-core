#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Object.hpp"
#include "unicore/Path.hpp"
#include "unicore/Containers.hpp"
#include "unicore/DateTime.hpp"

namespace unicore
{
	class MemoryChunk;
	class ReadStream;
	class WriteStream;

	enum class StreamFlag : uint8_t
	{
		None = 0,
		Directory = 1,
		File = 2,
	};
	UNICORE_ENUMFLAGS(StreamFlag, FileFlags);

	struct StreamStats
	{
		int64_t size = 0;
		DateTime modtime;
		DateTime createtime;
		DateTime accestime;
		StreamFlag flag = StreamFlag::None;
	};

	// BasicStreamProvider ////////////////////////////////////////////////////////
	class BasicStreamProvider : public Object
	{
		UC_OBJECT(BasicStreamProvider, Object)
	public:
		UC_NODISCARD virtual Optional<StreamStats> stats(const Path& path) const = 0;
		UC_NODISCARD virtual bool exists(const Path& path) const;

		virtual uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = StreamFlag::File | StreamFlag::Directory) const = 0;

		UC_NODISCARD virtual List<Path> enumerate(
			const Path& path, WStringView search_pattern,
			FileFlags flags = StreamFlag::File | StreamFlag::Directory) const;
	};

	// ReadStreamProvider /////////////////////////////////////////////////////////
	class ReadStreamProvider : public BasicStreamProvider
	{
		UC_OBJECT(ReadStreamProvider, BasicStreamProvider)
	public:
		UC_NODISCARD virtual Shared<ReadStream> open_read(const Path& path) = 0;
		UC_NODISCARD virtual Shared<MemoryChunk> read_chunk(const Path& path);
	};

	// WriteStreamProvider ////////////////////////////////////////////////////////
	class WriteStreamProvider : public ReadStreamProvider
	{
		UC_OBJECT(WriteStreamProvider, ReadStreamProvider)
	public:
		virtual bool create_directory(const Path& path) = 0;
		virtual bool delete_directory(const Path& path, bool recursive = false) = 0;

		virtual Shared<WriteStream> create_new(const Path& path) = 0;

		virtual bool delete_file(const Path& path) = 0;

		virtual bool write_chunk(const Path& path, const MemoryChunk& chunk);
	};

	// DirectoryStreamProvider ////////////////////////////////////////////////////
	class DirectoryStreamProvider : public ReadStreamProvider
	{
		UC_OBJECT(DirectoryStreamProvider, ReadStreamProvider)
	public:
		explicit DirectoryStreamProvider(ReadStreamProvider& provider, const Path& base)
			: _provider(provider), _base(base) {}

		UC_NODISCARD Optional<StreamStats> stats(const Path& path) const override;
		UC_NODISCARD bool exists(const Path& path) const override;

		uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = StreamFlag::File | StreamFlag::Directory) const override;

		Shared<ReadStream> open_read(const Path& path) override;

	protected:
		ReadStreamProvider& _provider;
		Path _base;

		UC_NODISCARD Path make_path(const Path& path) const { return _base / path; }
	};

	// ArchiveStreamProvider //////////////////////////////////////////////////////
	class ArchiveStreamProvider : public ReadStreamProvider, protected CachedPathData<intptr_t>
	{
		UC_OBJECT(ArchiveStreamProvider, ReadStreamProvider)
	public:
		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<StreamStats> stats(const Path& path) const override;

		uint16_t enumerate(const Path& path,
			WStringView search_pattern, List<Path>& name_list,
			FileFlags flags = StreamFlag::File | StreamFlag::Directory) const override;

		Shared<ReadStream> open_read(const Path& path) override;

	protected:
		UC_NODISCARD virtual Optional<StreamStats> stats_index(intptr_t index) const = 0;
		UC_NODISCARD virtual Shared<ReadStream> open_read_index(intptr_t index) = 0;
		UC_NODISCARD virtual bool enumerate_index(intptr_t index, FileFlags flags) const =0;
	};
}