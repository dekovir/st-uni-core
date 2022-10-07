#pragma once
#include "unicore/Math.hpp"

namespace unicore
{
	// TODO: Replace with filesystem::path
	class Path
	{
	public:
		using StringType = String;
		using StringViewType = StringView;

		Path();
		explicit Path(StringView path);
		explicit Path(StringView16 path);
		explicit Path(StringView32 path);

		// Copy constructor
		Path(const Path& other) = default;

		// Move constructor
		Path(Path&& other) noexcept;

		~Path() = default;

		// Copy assignment operator
		Path& operator=(const Path& other) = default;

		// Move assignment operator
		Path& operator=(Path&& other) noexcept;

		UC_NODISCARD bool absolute() const;
		UC_NODISCARD bool empty() const { return _data.empty(); }
		void clear();

		UC_NODISCARD size_t hash() const { return _hash; }
		UC_NODISCARD const String& data() const { return _data; }

		UC_NODISCARD bool has_extension() const;
		UC_NODISCARD bool has_extension(StringView ext) const;

		UC_NODISCARD bool starts_with(const Path& path) const;

		void parent_path(Path& parentPath) const;
		UC_NODISCARD Path parent_path() const;

		void filename(String& fileName) const;
		UC_NODISCARD String filename() const;

		void extension(String& ext) const;
		UC_NODISCARD String extension() const;

		void explode(Path& parentDir, String& fileName) const;
		void explode(Path& parentDir, String& fileName, String& extension) const;
		void explode(List<String>& elements) const;
		UC_NODISCARD List<String> explode() const;

		void replace_filename(StringView filename);
		void replace_extension(StringView ext);

		void remove_extension();

		UC_NODISCARD bool equals(const Path& other) const;
		UC_NODISCARD int compare(const Path& other) const;

		bool operator==(const Path& other) const { return equals(other); }
		bool operator!=(const Path& other) const { return !equals(other); }
		bool operator<(const Path& other) const { return compare(other) < 0; }
		bool operator>(const Path& other) const { return compare(other) > 0; }

		Path operator/(const String& file) const;
		Path operator/(const Path& path) const;

		void native_path(String& nativePath) const;
		UC_NODISCARD String native_path() const;

		UC_NODISCARD Path to_lower() const;

		static Path combine(StringView a, StringView b);
		static Path combine(StringView a, StringView b, StringView c);

		static const Char DirSeparator;
		static const Char NativeDirSeparator;

		static const Path Empty;

		//static constexpr WStringView WildcardExt = L".*";

	protected:
		String _data;
		size_t _hash;

		Path(StringView path, size_t hash);

		static String::size_type find_filename_pos(StringView str);
		static String::size_type find_extension_pos(StringView str);

		static String prepare(StringView str);
		static size_t calc_hash(StringView str);
	};

	static Path operator/(const Path& path, StringView file)
	{
		return Path::combine(path.data(), file);
	}

	static Path operator"" _path(const char* path, size_t len)
	{
		return Path(StringView(path, len));
	}

		static Path operator"" _path(const Char32* path, size_t len)
	{
		return Path(StringView32(path, len));
	}


	extern UNICODE_STRING_BUILDER_FORMAT(const Path&);

	UNICORE_MATH_HASH(Path)
	{
		return hash(value.data());
	}
}