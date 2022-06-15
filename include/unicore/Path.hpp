#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Path
	{
	public:
		Path() = default;
		explicit Path(const WString& path);

		[[nodiscard]] bool absolute() const;
		[[nodiscard]] bool empty() const { return _path.empty(); }
		void clear();

		[[nodiscard]] size_t hash() const { return _hash; }
		[[nodiscard]] const WString& data() const { return _path; }

		[[nodiscard]] bool has_extension() const;

		void parent_path(Path& parentPath) const;
		[[nodiscard]] Path parent_path() const;

		void filename(WString& fileName) const;
		[[nodiscard]] WString filename() const;

		void extension(WString& ext) const;
		[[nodiscard]] WString extension() const;

		void explode(Path& parentDir, WString& fileName) const;
		void explode(Path& parentDir, WString& fileName, WString& extension) const;
		void explode(List<WString>& elements) const;

		void replace_filename(const WString& filename);
		void replace_extension(const WString& ext);
		void remove_extension(const List<WString>& extensions);

		[[nodiscard]] bool equals(const Path& other) const;
		[[nodiscard]] int compare(const Path& other) const;

		bool operator==(const Path& other) const { return equals(other); }
		bool operator!=(const Path& other) const { return !equals(other); }
		bool operator<(const Path& other) const { return compare(other) < 0; }
		bool operator>(const Path& other) const { return compare(other) > 0; }

		Path operator/(const WString& file) const;
		Path operator/(const Path& path) const;

		void native_path(WString& nativePath) const;
		[[nodiscard]] WString native_path() const;

		[[nodiscard]] Path to_lower() const;

		static Path combine(const WString& a, const WString& b);
		static Path combine(const WString& a, const WString& b, const WString& c);

		static const wchar_t DirSeparator;
		static const wchar_t NativeDirSeparator;

		static const Path Empty;

	protected:
		WString _path;
		size_t _hash = 0;

		Path(const WString& path, size_t hash);

		static WString::size_type find_drive_pos(const WString& str);
		static WString::size_type find_filename_pos(const WString& str);
		static WString::size_type find_extension_pos(const WString& str);

		static WString prepare(const WString& str);
		static size_t calc_hash(const WString& str);
	};

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const Path& path);
}