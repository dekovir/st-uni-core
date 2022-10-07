#pragma once
#include "unicore/Resource.hpp"

namespace unicore
{
	class MemoryChunk;
	class BinaryData;

	enum class SeekMethod
	{
		Begin,
		Current,
		End,
	};

	class File : public Resource
	{
		UC_OBJECT(File, Resource)
	public:
		UC_NODISCARD size_t get_system_memory_use() const override { return sizeof(File); }
		UC_NODISCARD ResourceCachePolicy cache_policy() const override { return ResourceCachePolicy::NoCache; }

		UC_NODISCARD virtual int64_t size() const = 0;
		virtual int64_t seek(int64_t offset, SeekMethod method = SeekMethod::Begin) = 0;
	};

	class ReadFile : public File
	{
		UC_OBJECT(ReadFile, File)
	public:
		UC_NODISCARD virtual bool eof() const = 0;
		virtual bool read(void* buffer, size_t size, size_t* bytes_read = nullptr) = 0;

		bool read(MemoryChunk& chunk, size_t* bytes_read = nullptr);

		Shared<BinaryData> as_data();
	};

	enum class WriteFileMode : uint8_t
	{
		// Creates a new file, always.
		CreateAlways,
		// Creates a new file, only if it does not already exist.
		CreateNew,
	};

	class WriteFileOptions : public ResourceOptions
	{
	public:
		WriteFileMode mode;

		UC_NODISCARD size_t hash() const override { return static_cast<size_t>(mode); }

		explicit WriteFileOptions(WriteFileMode mode = WriteFileMode::CreateAlways)
			: mode(mode)
		{
		}
	};

	class WriteFile : public ReadFile
	{
		UC_OBJECT(WriteFile, ReadFile)
	public:
		virtual bool flush() = 0;
		virtual bool write(const void* buffer, size_t size, size_t* bytes_written = nullptr) = 0;

		bool write(const MemoryChunk& chunk, size_t* bytes_written = nullptr);
	};

	// FileReader /////////////////////////////////////////////////////////////////
	class FileReader
	{
	public:
		ReadFile& stream;

		explicit FileReader(ReadFile& stream_)
			: stream(stream_) {}

		UC_NODISCARD bool eof() const { return stream.eof(); }

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		bool read(T& value)
		{
			return stream.read(&value, sizeof(T));
		}

		template<typename Char>
		bool read_string_nt(BasicString<Char>& str)
		{
			if (eof()) return false;

			size_t count = 0;
			Char c;

			while (stream.read(&c, sizeof(Char)))
			{
				if (c == 0)
					break;

				str.append(c);
				count++;
			}

			return count > 0;
		}
	};

	// FileWriter /////////////////////////////////////////////////////////////////
	class FileWriter
	{
	public:
		WriteFile& stream;

		explicit FileWriter(WriteFile& stream_)
			: stream(stream_)
		{
		}

		FileWriter& write(StringView str);
		FileWriter& write(StringView16 str);
		FileWriter& write(StringView32 str);

		template<typename T,
			std::enable_if_t<std::is_integral_v<T>>* = nullptr>
		FileWriter& write(T value)
		{
			stream.write(&value, sizeof(value));
			return *this;
		}

		template<typename Char>
		FileWriter& write_string_nt(BasicStringView<Char> str)
		{
			stream.write(str.data(), sizeof(Char) * (str.size() + 1));
			return *this;
		}
	};
}