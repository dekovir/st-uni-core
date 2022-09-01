#pragma once
#include "unicore/EnumFlag.hpp"
#include "unicore/Object.hpp"
#include "unicore/Path.hpp"
#include "unicore/Containers.hpp"
#include "unicore/DateTime.hpp"

namespace unicore
{
	class MemoryChunk;
	class ReadFile;
	class WriteFile;

	enum class FileFlag : uint8_t
	{
		None = 0,
		Directory = 1,
		File = 2,
	};
	UNICORE_ENUMFLAGS(FileFlag, FileFlags);

	struct FileStats
	{
		int64_t size = 0;
		DateTime modtime;
		DateTime createtime;
		DateTime accestime;
		FileFlag flag = FileFlag::None;
	};

	// FileProvider ///////////////////////////////////////////////////////////////
	class FileProvider : public Object
	{
		UC_OBJECT(FileProvider, Object)
	public:
		UC_NODISCARD virtual Optional<FileStats> stats(const Path& path) const = 0;
		UC_NODISCARD virtual bool exists(const Path& path) const;

		virtual uint16_t enumerate_entries(const Path& path,
			WStringView search_pattern, List<WString>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const = 0;

		uint16_t enumerate_files(const Path& path,
			WStringView search_pattern, List<WString>& name_list) const;

		uint16_t enumerate_dirs(const Path& path,
			WStringView search_pattern, List<WString>& name_list) const;

		UC_NODISCARD virtual List<WString> get_files(
			const Path& path, WStringView search_pattern) const;

		UC_NODISCARD virtual List<WString> get_dirs(
			const Path& path, WStringView search_pattern) const;
	};

	// ReadFileProvider ///////////////////////////////////////////////////////////
	class ReadFileProvider : public FileProvider
	{
		UC_OBJECT(ReadFileProvider, FileProvider)
	public:
		UC_NODISCARD virtual Shared<ReadFile> open_read(const Path& path) = 0;
		UC_NODISCARD virtual Shared<MemoryChunk> read_chunk(const Path& path);
	};

	// WriteFileProvider /////////////////////////////////////////////////////////
	class WriteFileProvider : public ReadFileProvider
	{
		UC_OBJECT(WriteFileProvider, ReadFileProvider)
	public:
		virtual bool create_directory(const Path& path) = 0;
		virtual bool delete_directory(const Path& path, bool recursive = false) = 0;

		virtual Shared<WriteFile> create_new(const Path& path) = 0;

		virtual bool delete_file(const Path& path) = 0;

		virtual bool write_chunk(const Path& path, const MemoryChunk& chunk);
	};

	// DirectoryFileProvider //////////////////////////////////////////////////////
	class DirectoryFileProvider : public ReadFileProvider
	{
		UC_OBJECT(DirectoryFileProvider, ReadFileProvider)
	public:
		explicit DirectoryFileProvider(ReadFileProvider& provider, const Path& base)
			: _provider(provider), _base(base) {}

		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;
		UC_NODISCARD bool exists(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path,
			WStringView search_pattern, List<WString>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const override;

		Shared<ReadFile> open_read(const Path& path) override;

	protected:
		ReadFileProvider& _provider;
		Path _base;

		UC_NODISCARD Path make_path(const Path& path) const { return _base / path; }
	};

	// ArchiveFileProvider ////////////////////////////////////////////////////////
	class ArchiveFileProvider : public ReadFileProvider, protected CachedPathData<intptr_t>
	{
		UC_OBJECT(ArchiveFileProvider, ReadFileProvider)
	public:
		UC_NODISCARD bool exists(const Path& path) const override;
		UC_NODISCARD Optional<FileStats> stats(const Path& path) const override;

		uint16_t enumerate_entries(const Path& path,
			WStringView search_pattern, List<WString>& name_list,
			FileFlags flags = FileFlag::File | FileFlag::Directory) const override;

		Shared<ReadFile> open_read(const Path& path) override;

	protected:
		UC_NODISCARD virtual Optional<FileStats> stats_index(intptr_t index) const = 0;
		UC_NODISCARD virtual Shared<ReadFile> open_read_index(intptr_t index) = 0;
		UC_NODISCARD virtual bool enumerate_index(intptr_t index, FileFlags flags) const =0;
	};
}