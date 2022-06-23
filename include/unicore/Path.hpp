#pragma once
#include "unicore/Defs.hpp"

namespace unicore
{
	class Path
	{
	public:
		Path();
		explicit Path(WStringView path);

		UC_NODISCARD bool absolute() const;
		UC_NODISCARD bool empty() const { return _data.empty(); }
		void clear();

		UC_NODISCARD size_t hash() const { return _hash; }
		UC_NODISCARD const WString& data() const { return _data; }

		UC_NODISCARD bool has_extension() const;

		void parent_path(Path& parentPath) const;
		UC_NODISCARD Path parent_path() const;

		void filename(WString& fileName) const;
		UC_NODISCARD WString filename() const;

		void extension(WString& ext) const;
		UC_NODISCARD WString extension() const;

		void explode(Path& parentDir, WString& fileName) const;
		void explode(Path& parentDir, WString& fileName, WString& extension) const;
		void explode(List<WString>& elements) const;
		UC_NODISCARD List<WString> explode() const;

		void replace_filename(WStringView filename);
		void replace_extension(WStringView ext);
		void remove_extension(const List<WString>& extensions);

		UC_NODISCARD bool equals(const Path& other) const;
		UC_NODISCARD int compare(const Path& other) const;

		bool operator==(const Path& other) const { return equals(other); }
		bool operator!=(const Path& other) const { return !equals(other); }
		bool operator<(const Path& other) const { return compare(other) < 0; }
		bool operator>(const Path& other) const { return compare(other) > 0; }

		Path operator/(const WString& file) const;
		Path operator/(const Path& path) const;

		void native_path(WString& nativePath) const;
		[[nodiscard]] WString native_path() const;

		[[nodiscard]] Path to_lower() const;

		static Path combine(WStringView a, WStringView b);
		static Path combine(WStringView a, WStringView b, WStringView c);

		static const wchar_t DirSeparator;
		static const wchar_t NativeDirSeparator;

		static const Path Empty;

	protected:
		WString _data;
		size_t _hash;

		Path(WStringView path, size_t hash);

		static WString::size_type find_drive_pos(WStringView str);
		static WString::size_type find_filename_pos(WStringView str);
		static WString::size_type find_extension_pos(WStringView str);

		static WString prepare(WStringView str);
		static size_t calc_hash(WStringView str);
	};

	static Path operator"" _path(const wchar_t* path, size_t len)
	{
		return Path(WStringView(path, len));
	}

	class LogHelper;
	extern LogHelper& operator << (LogHelper& helper, const Path& path);
}